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
        Renderer::Shutdown();
    }

    void Application::Run()
    {
        while (m_running)
        {
            float time = (float) glfwGetTime(); // should be Platform::GetTime()

            TimeStep timeStep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            if (!m_minimized)
            {
                for (Layer* layer : m_layerStack)
                    layer->OnUpdate(timeStep);
             
            }

            m_imGuiLayer->Begin(); 

            for (Layer* layer : m_layerStack)
                layer->OnImGuiRender();

            m_imGuiLayer->End();
            m_window->OnUpdate();
        }
    }
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(RE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(Application::OnWindowResize));

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
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