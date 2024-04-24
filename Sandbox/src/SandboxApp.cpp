#include <REngine.h>
#include <REngine/Core/EntryPoint.h>

#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>

#include <glm/ext/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public REngine::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_cameraController(1280.0f / 720.0f, true)
    {
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
        };

        uint32_t indices[3] = { 0, 1, 2 };

        float squareVertices[5 * 4] =
        {
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
           -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_transform;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = a_position;
                v_color = a_color;
                gl_Position = u_viewProjectionMatrix * u_transform * vec4(a_position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec3 v_position;
            in vec4 v_color;

            void main()
            {
                color = vec4(v_position * 0.5 + 0.5, 1.0);
                color = v_color;
            }
        )";
        std::string flatColorShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_transform;

            out vec3 v_position;

            void main()
            {
                v_position = a_position;
                gl_Position = u_viewProjectionMatrix * u_transform * vec4(a_position, 1.0);
            }
        )";

        std::string flatColorShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec3 v_position;

            uniform vec3 u_color;

            void main()
            {
                color = vec4(u_color, 1.0);
            }
        )";

        REngine::BufferLayout layout =
        {
            { REngine::ShaderDataType::Float3, "a_position" },
            { REngine::ShaderDataType::Float4, "a_color" }
        };

        REngine::BufferLayout squareLayout =
        {
            { REngine::ShaderDataType::Float3, "a_position" },
            { REngine::ShaderDataType::Float2, "a_texCoord" },
        };

        m_vertexArray = REngine::VertexArray::Create();

        REngine::Ref<REngine::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(REngine::VertexBuffer::Create(vertices, sizeof(vertices)));

        REngine::Ref<REngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(REngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        vertexBuffer->SetLayout(layout);

        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);

        m_shader = REngine::Shader::Create("TriangleShader", vertexSrc, fragmentSrc);


        m_squareVertexArray = REngine::VertexArray::Create();

        REngine::Ref<REngine::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(REngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        REngine::Ref<REngine::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(REngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

        squareVertexBuffer->SetLayout(squareLayout);

        m_squareVertexArray->AddVertexBuffer(squareVertexBuffer);
        m_squareVertexArray->SetIndexBuffer(squareIndexBuffer);

        m_flatColorShader = REngine::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

        auto textureShader = m_shaderLibrary.Load("Assets/Shaders/Texture.glsl");

        m_texture2D = REngine::Texture2D::Create("Assets/Textures/AAAAA.png");
        m_logoTexture2D = REngine::Texture2D::Create("Assets/Textures/REngineLogo.png");

        std::dynamic_pointer_cast<REngine::OpenGLShader>(textureShader)->Bind();
        std::dynamic_pointer_cast<REngine::OpenGLShader>(textureShader)->UploadUniformInt("u_texture", 0);
    }

    void OnUpdate(REngine::TimeStep ts) override
    {
        // Update
        m_cameraController.OnUpdate(ts);

        // Render
        REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        REngine::RenderCommand::Clear();

        REngine::Renderer::BeginScene(m_cameraController.GetCamera());

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        glm::vec4 greenColor(0.3, 0.8f, 0.2f, 1.0f);
        glm::vec4 blueColor(0.2, 0.3f, 0.8f, 1.0f);

        std::dynamic_pointer_cast<REngine::OpenGLShader>(m_flatColorShader)->Bind();
        std::dynamic_pointer_cast<REngine::OpenGLShader>(m_flatColorShader)->UploadUniformFloat3("u_color", m_squareColor);

        for (int x = 0; x < 20; x++)
        {
            for (int y = 0; y < 20; y++)
            {
                glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
                REngine::Renderer::Submit(m_flatColorShader, m_squareVertexArray, transform);
            }
        }

        auto textureShader = m_shaderLibrary.Get("Texture");

        m_texture2D->Bind();
        REngine::Renderer::Submit(textureShader, m_squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        m_logoTexture2D->Bind();
        REngine::Renderer::Submit(textureShader, m_squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        
        // Rendering Triangle
        //REngine::Renderer::Submit(m_shader, m_vertexArray);
        REngine::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
        ImGui::End();
    }

    void OnEvent(REngine::Event& e) override
    {
        m_cameraController.OnEvent(e);
    }

private:

    REngine::ShaderLibrary m_shaderLibrary;

    REngine::Ref<REngine::Shader> m_shader;
    REngine::Ref<REngine::VertexArray> m_vertexArray;

    REngine::Ref<REngine::Shader> m_flatColorShader;
    REngine::Ref<REngine::VertexArray> m_squareVertexArray;

    REngine::Ref<REngine::Texture2D> m_texture2D;
    REngine::Ref<REngine::Texture2D> m_logoTexture2D;

    REngine::OrthographicCameraController m_cameraController;

    glm::vec3 m_squareColor = { 0.2f, 0.8f, 0.3f };
};

class Sandbox : public REngine::Application
{
public:
    Sandbox()
    {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {

    }
};

REngine::Application* REngine::CreateApplication()
{
    return new Sandbox();
}