#include "ExampleLayer.h"
#include <imgui/imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


ExampleLayer::ExampleLayer() 
    : Layer("Example"), m_CameraController(1280.0f / 720.0f)
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

    m_VertexArray = REngine::VertexArray::Create();

    REngine::Ref<REngine::VertexBuffer> vertexBuffer = REngine::VertexBuffer::Create(vertices, sizeof(vertices));

    REngine::Ref<REngine::IndexBuffer> indexBuffer = REngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    vertexBuffer->SetLayout(layout);

    m_VertexArray->AddVertexBuffer(vertexBuffer);
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_Shader = REngine::Shader::Create("TriangleShader", vertexSrc, fragmentSrc);


    m_SquareVertexArray = REngine::VertexArray::Create();

    REngine::Ref<REngine::VertexBuffer> squareVertexBuffer = REngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

    REngine::Ref<REngine::IndexBuffer> squareIndexBuffer = REngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

    squareVertexBuffer->SetLayout(squareLayout);

    m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

    m_FlatColorShader = REngine::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

    auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/Texture.glsl");

    m_Texture2D = REngine::Texture2D::Create("Assets/Textures/AAAAA.png");
    m_LogoTexture2D = REngine::Texture2D::Create("Assets/Textures/REngineLogo.png");

    textureShader->Bind();
    textureShader->SetInt("u_texture", 0);
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(REngine::TimeStep ts)
{
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    REngine::RenderCommand::Clear();

    REngine::Renderer::BeginScene(m_CameraController.GetCamera());

    static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    glm::vec4 greenColor(0.3, 0.8f, 0.2f, 1.0f);
    glm::vec4 blueColor(0.2, 0.3f, 0.8f, 1.0f);

    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat3("u_color", m_SquareColor);

    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
            REngine::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
        }
    }

    auto textureShader = m_ShaderLibrary.Get("Texture");

    m_Texture2D->Bind();
    REngine::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
    m_LogoTexture2D->Bind();
    REngine::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
        
    // Rendering Triangle
    //REngine::Renderer::Submit(m_shader, m_vertexArray);
    REngine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender() 
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void ExampleLayer::OnEvent(REngine::Event& e) 
{
    m_CameraController.OnEvent(e);
}
