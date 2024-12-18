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
    RE_PROFILE_FUNCTION();

    m_texture2D = REngine::Texture2D::Create("Assets/Textures/AAAAA.png");
    m_logo = REngine::Texture2D::Create("Assets/Textures/R-Engine_Logo.png");
}

void Sandbox2D::OnDetach()
{
    RE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(REngine::TimeStep ts)
{
    RE_PROFILE_FUNCTION();

    m_cameraController.OnUpdate(ts);
    REngine::Renderer2D::ResetStats();

    {
        RE_PROFILE_SCOPE("Renderer Prep");
        REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        REngine::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * -50;

        RE_PROFILE_SCOPE("Renderer Draw");
        REngine::Renderer2D::BeginScene(m_cameraController.GetCamera());
        REngine::Renderer2D::DrawQuadRotated({ -3.0f, 1.0f}, glm::radians(22.5f), { 0.8f, 0.8f }, { 0.3, 1.0f, 0.4f, 1.0f });
        REngine::Renderer2D::DrawQuad({ 2.0f, -1.0f }, { 1.0f, 1.5f }, m_squareColor);
        REngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 5.0f, 5.0f }, m_texture2D, 10.0f);
        REngine::Renderer2D::DrawQuadRotated({ 0.0f, 0.0f, 1.0f}, glm::radians(rotation), {1.0f, 1.0f}, m_logo, 1.0f);
        REngine::Renderer2D::EndScene();

        REngine::Renderer2D::BeginScene(m_cameraController.GetCamera());
        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.75f };
                REngine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
            }

        }
        REngine::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    RE_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = REngine::Renderer2D::GetStats();

    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quad Count: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

    ImGui::End();
}

void Sandbox2D::OnEvent(REngine::Event& e)
{
    m_cameraController.OnEvent(e);
}
