#include <REngine.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/ext/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public REngine::Layer
{
public:
    ExampleLayer()
        :Layer("Example"), camera(-1.6f, 1.6f, -.9f, 0.9f), m_cameraPosition(0.0f)
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

        std::string textureShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec2 a_texCoord;

            uniform mat4 u_viewProjectionMatrix;
            uniform mat4 u_transform;

            out vec2 v_texCoord;

            void main()
            {
                v_texCoord = a_texCoord;
                gl_Position = u_viewProjectionMatrix * u_transform * vec4(a_position, 1.0);
            }
        )";

        std::string textureShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec2 v_texCoord;

            uniform sampler2D u_texture;

            void main()
            {
                color = texture(u_texture, v_texCoord);
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

        m_vertexArray.reset(REngine::VertexArray::Create());

        REngine::Ref<REngine::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(REngine::VertexBuffer::Create(vertices, sizeof(vertices)));

        REngine::Ref<REngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(REngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        vertexBuffer->SetLayout(layout);

        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);

        m_shader.reset(REngine::Shader::Create(vertexSrc, fragmentSrc));


        m_squareVertexArray.reset(REngine::VertexArray::Create());

        REngine::Ref<REngine::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(REngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        REngine::Ref<REngine::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(REngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

        squareVertexBuffer->SetLayout(squareLayout);

        m_squareVertexArray->AddVertexBuffer(squareVertexBuffer);
        m_squareVertexArray->SetIndexBuffer(squareIndexBuffer);

        m_flatColorShader.reset(REngine::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

        m_textureShader.reset(REngine::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

        m_texture2D = REngine::Texture2D::Create("assets/textures/AAAAA.png");
        m_logoTexture2D = REngine::Texture2D::Create("assets/textures/REngineLogo.png");

        std::dynamic_pointer_cast<REngine::OpenGLShader>(m_textureShader)->Bind();
        std::dynamic_pointer_cast<REngine::OpenGLShader>(m_textureShader)->UploadUniformInt("u_texture", 0);
    }

    void OnUpdate(REngine::TimeStep ts) override
    {
        if (REngine::Input::IsKeyPressed(RE_KEY_A))
            m_cameraPosition.x -= m_cameraMoveSpeed * ts;
        if (REngine::Input::IsKeyPressed(RE_KEY_D))
            m_cameraPosition.x += m_cameraMoveSpeed * ts;
        if (REngine::Input::IsKeyPressed(RE_KEY_W))
            m_cameraPosition.y += m_cameraMoveSpeed * ts;
        if (REngine::Input::IsKeyPressed(RE_KEY_S))
            m_cameraPosition.y -= m_cameraMoveSpeed * ts;

        if (REngine::Input::IsKeyPressed(RE_KEY_Q))
            m_cameraRotation += m_cameraRotationSpeed * ts;
        if (REngine::Input::IsKeyPressed(RE_KEY_E))
            m_cameraRotation -= m_cameraRotationSpeed * ts;

        REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        REngine::RenderCommand::Clear();

        camera.SetPosition(m_cameraPosition);
        camera.SetRotation(m_cameraRotation);

        REngine::Renderer::BeginScene(camera);

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

        m_texture2D->Bind();
        REngine::Renderer::Submit(m_textureShader, m_squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        m_logoTexture2D->Bind();
        REngine::Renderer::Submit(m_textureShader, m_squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        
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

    void OnEvent(REngine::Event& event) override
    {
    }

private:
    REngine::Ref<REngine::Shader> m_shader;
    REngine::Ref<REngine::VertexArray> m_vertexArray;

    REngine::Ref<REngine::Shader> m_flatColorShader;
    REngine::Ref<REngine::Shader> m_textureShader;
    REngine::Ref<REngine::VertexArray> m_squareVertexArray;

    REngine::Ref<REngine::Texture2D> m_texture2D;
    REngine::Ref<REngine::Texture2D> m_logoTexture2D;

    REngine::OrthographicCamera camera;
    glm::vec3 m_cameraPosition;

    float m_cameraRotation = 0;
    float m_cameraMoveSpeed = 5.0f;
    float m_cameraRotationSpeed = 20.0f;

    glm::vec3 m_squareColor = { 0.2f, 0.8f, 0.3f };
};

class Sandbox : public REngine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

REngine::Application* REngine::CreateApplication()
{
    return new Sandbox();
}