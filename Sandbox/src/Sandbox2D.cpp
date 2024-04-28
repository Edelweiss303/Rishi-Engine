#include "Sandbox2D.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Platform/OpenGL/OpenGLShader.h>

template<typename Fn>
class Timer
{
public: 
    Timer(const char* name, Fn&& func)
        : m_name(name), m_func(func), m_stopped(false)
    {
        m_startTimePoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimePoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimePoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

        m_stopped = true;

        float duration = (end - start) * 0.001f;

        m_func({ m_name, duration });
    }

private:
    const char* m_name;
    Fn m_func;
    std::chrono::time_point<std::chrono::steady_clock> m_startTimePoint;
    bool m_stopped;
};
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    m_texture2D = REngine::Texture2D::Create("Assets/Textures/AAAAA.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(REngine::TimeStep ts)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate");

    {
        PROFILE_SCOPE("CameraController::OnUpdate");
        m_cameraController.OnUpdate(ts);
    }

    {
        PROFILE_SCOPE("Renderer Prep");
        REngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        REngine::RenderCommand::Clear();
    }

    {
        PROFILE_SCOPE("Renderer Draw");
        REngine::Renderer2D::BeginScene(m_cameraController.GetCamera());
        REngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.3, 1.0f, 0.4f, 1.0f });
        REngine::Renderer2D::DrawQuad({ 0.0f, 1.0f }, 5.0f, { 1.0f, 1.5f }, { 0.3, 0.4f, 1.0f, 1.0f });
        REngine::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 5.0f, 5.0f }, m_texture2D);
        REngine::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

    for (auto& result : m_profileResults)
    {
        char label[50];
        strcpy(label, "  %.3fms ");
        strcat(label, result.Name);
        ImGui::Text(label, result.Time);
    }

    m_profileResults.clear();

    ImGui::End();
}

void Sandbox2D::OnEvent(REngine::Event& e)
{
    m_cameraController.OnEvent(e);
}
