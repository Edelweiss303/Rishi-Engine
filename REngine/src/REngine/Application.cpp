#include "repch.h"
#include "Application.h"

#include "REngine/Events/ApplicationEvent.h"
#include "REngine/log.h"

namespace REngine
{
    Application::Application()
    {
        m_window = std::unique_ptr<Window>(Window::Create());
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_running)
        {
            m_window->OnUpdate();
        }
    }
}