#include "repch.h"
#include "Application.h"

#include "REngine/Events/ApplicationEvent.h"
#include "REngine/log.h"

namespace REngine
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        WindowResizeEvent e(1920, 1080);
        RE_TRACE(e);
        while (true);
    }
}