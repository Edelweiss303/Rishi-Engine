#include "REpch.h"
#include "Application.h"

#include "log.h"

#include "REngine/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace REngine
{
    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        RE_PROFILE_FUNCTION();

        RE_CORE_ASSERT(!s_instance, "Application already exists!")
        s_instance = this;

        m_window = Window::Create();
        m_window->SetEventCallback(RE_BIND_EVENT_FN(Application::OnEvent));

        Renderer::Init();

        m_imGuiLayer = new ImGuiLayer();
        PushOverlay(m_imGuiLayer);
    }

    Application::~Application()
    {
        RE_PROFILE_FUNCTION();

        Renderer::Shutdown();
    }

    void Application::Run()
    {
        RE_PROFILE_FUNCTION();

        while (m_running)
        {
            RE_PROFILE_SCOPE("RunLoop");

            float time = (float) glfwGetTime(); // should be Platform::GetTime()

            TimeStep timeStep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            if (!m_minimized)
            {
                {
                    RE_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer* layer : m_layerStack)
                        layer->OnUpdate(timeStep);
                }

                m_imGuiLayer->Begin(); 

                {
                    RE_PROFILE_SCOPE("LayerStack OnImGuiRender");

                    for (Layer* layer : m_layerStack)
                        layer->OnImGuiRender();
                }

                m_imGuiLayer->End();
            }

            m_window->OnUpdate();
        }
    }
    void Application::OnEvent(Event& e)
    {
        RE_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(RE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); it++)
        {
            (*it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        RE_PROFILE_FUNCTION();

        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        RE_PROFILE_FUNCTION();

        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        RE_PROFILE_FUNCTION();

        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_minimized = true;
            return false;
        }

        m_minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
}