#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "REngine/LayerStack.h"
#include "REngine/Events/ApplicationEvent.h"
#include "Input.h"

#include "REngine/ImGui/ImguiLayer.h"

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

        static inline Application& Get() { return *s_instance; }
        inline Window& GetWindow() { return *m_window; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        unsigned int m_vertexArray;
        unsigned int m_vertexBuffer;
        unsigned int m_indexBuffer;

    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_running = true;
        LayerStack m_layerStack;

        static Application* s_instance;
    };

    //To be defined in client
    Application* CreateApplication();
}

