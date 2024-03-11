#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "REngine/LayerStack.h"
#include "REngine/Events/ApplicationEvent.h"

namespace REngine
{
    class RE_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_window;
        bool m_running = true;
        LayerStack m_layerStack;
    };

    //To be defined in client
    Application* CreateApplication();
}

