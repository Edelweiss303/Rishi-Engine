#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"

namespace REngine
{
    class RE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        std::unique_ptr<Window> m_window;
        bool m_running = true;
    };

    //To be defined in client
    Application* CreateApplication();
}

