#pragma once
#include "Core.h"

#include "Window.h"
#include "REngine/Core/LayerStack.h"
#include "REngine/Events/ApplicationEvent.h"
#include "REngine/Core/Timestep.h"

#include "REngine/ImGui/ImguiLayer.h"

namespace REngine
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        static Application& Get() { return *s_instance; }
        Window& GetWindow() { return *m_window; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        static Application* s_instance;
        
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        bool m_running = true;
        bool m_minimized = false;
        LayerStack m_layerStack;
        float m_lastFrameTime = 0.0f;
    };

    //To be defined in client
    Application* CreateApplication();
}

