#include <REngine.h>
#include "imgui/imgui.h"
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

        float squareVertices[3 * 4] =
        {
           -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f,  0.75f, 0.0f,
           -0.75f,  0.75f, 0.0f
        };

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            uniform mat4 viewProjectionMatrix;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = a_position;
                v_color = a_color;
                gl_Position = viewProjectionMatrix * vec4(a_position, 1.0);
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
        std::string blueShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;

            uniform mat4 viewProjectionMatrix;

            out vec3 v_position;

            void main()
            {
                v_position = a_position;
                gl_Position = viewProjectionMatrix * vec4(a_position, 1.0);
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec3 v_position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        REngine::BufferLayout layout =
        {
            { REngine::ShaderDataType::Float3, "a_position" },
            { REngine::ShaderDataType::Float4, "a_color" }
        };

        REngine::BufferLayout squareLayout =
        {
            { REngine::ShaderDataType::Float3, "a_position" }
        };

        m_vertexArray.reset(REngine::VertexArray::Create());

        std::shared_ptr<REngine::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(REngine::VertexBuffer::Create(vertices, sizeof(vertices)));

        std::shared_ptr<REngine::IndexBuffer> indexBuffer;
        indexBuffer.reset(REngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        vertexBuffer->SetLayout(layout);

        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);

        m_shader.reset(REngine::Shader::Create(vertexSrc, fragmentSrc));


        m_squareVertexArray.reset(REngine::VertexArray::Create());

        std::shared_ptr<REngine::VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(REngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        std::shared_ptr<REngine::IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(REngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

        squareVertexBuffer->SetLayout(squareLayout);

        m_squareVertexArray->AddVertexBuffer(squareVertexBuffer);
        m_squareVertexArray->SetIndexBuffer(squareIndexBuffer);

        m_blueShader.reset(REngine::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    void OnUpdate() override
    {
        if (REngine::Input::IsKeyPressed(RE_KEY_LEFT))
            m_cameraPosition.x -= m_cameraMoveSpeed;
        if (REngine::Input::IsKeyPressed(RE_KEY_RIGHT))
            m_cameraPosition.x += m_cameraMoveSpeed;
        if (REngine::Input::IsKeyPressed(RE_KEY_UP))
            m_cameraPosition.y += m_cameraMoveSpeed;
        if (REngine::Input::IsKeyPressed(RE_KEY_DOWN))
            m_cameraPosition.y -= m_cameraMoveSpeed;

        if (REngine::Input::IsKeyPressed(RE_KEY_Q))
            m_cameraRotation += m_cameraRotationSpeed;
        if (REngine::Input::IsKeyPressed(RE_KEY_E))
            m_cameraRotation -= m_cameraRotationSpeed;

        REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        REngine::RenderCommand::Clear();

        camera.SetPosition(m_cameraPosition);
        camera.SetRotation(m_cameraRotation);

        REngine::Renderer::BeginScene(camera);

        REngine::Renderer::Submit(m_blueShader, m_squareVertexArray);
        REngine::Renderer::Submit(m_shader, m_vertexArray);

        REngine::Renderer::EndScene();
    }

    void OnEvent(REngine::Event& event) override
    {
    }

private:
    std::shared_ptr<REngine::Shader> m_shader;
    std::shared_ptr<REngine::VertexArray> m_vertexArray;

    std::shared_ptr<REngine::Shader> m_blueShader;
    std::shared_ptr<REngine::VertexArray> m_squareVertexArray;

    REngine::OrthographicCamera camera;
    glm::vec3 m_cameraPosition;
    float m_cameraRotation = 0;
    float m_cameraMoveSpeed = 0.05f;
    float m_cameraRotationSpeed = 1.0f;

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