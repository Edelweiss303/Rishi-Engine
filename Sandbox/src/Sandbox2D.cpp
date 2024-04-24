#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Platform/OpenGL/OpenGLShader.h>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    float squareVertices[5 * 3] =
    {
       -0.5f, -0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f, 
        0.5f,  0.5f, 0.0f, 
       -0.5f,  0.5f, 0.0f 
    };

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

    m_squareVertexArray = REngine::VertexArray::Create();

    REngine::Ref<REngine::VertexBuffer> squareVertexBuffer;
    squareVertexBuffer.reset(REngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    REngine::Ref<REngine::IndexBuffer> squareIndexBuffer;
    squareIndexBuffer.reset(REngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

    squareVertexBuffer->SetLayout({
        { REngine::ShaderDataType::Float3, "a_position" }
        });

    m_squareVertexArray->AddVertexBuffer(squareVertexBuffer);
    m_squareVertexArray->SetIndexBuffer(squareIndexBuffer);

    m_flatColorShader = REngine::Shader::Create("Assets/Shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(REngine::TimeStep ts)
{
    m_cameraController.OnUpdate(ts);

    REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    REngine::RenderCommand::Clear();

    REngine::Renderer::BeginScene(m_cameraController.GetCamera());

    std::dynamic_pointer_cast<REngine::OpenGLShader>(m_flatColorShader)->Bind();
    std::dynamic_pointer_cast<REngine::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_color", m_squareColor);

    REngine::Renderer::Submit(m_flatColorShader, m_squareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    REngine::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(REngine::Event& e)
{
    m_cameraController.OnEvent(e);
}
