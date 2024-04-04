#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Window.h"
#include "REngine/LayerStack.h"
#include "REngine/Events/ApplicationEvent.h"
#include "Input.h"

#include "REngine/ImGui/ImguiLayer.h"

#include "REngine/Renderer/VertexArray.h"
#include "REngine/Renderer/Shader.h"
#include "REngine/Renderer/Buffer.h"
#include "REngine/Renderer/Renderer.h"
#include "REngine/Renderer/OrthographicCamera.h"

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

        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<VertexArray> m_vertexArray;

        std::shared_ptr<Shader> m_blueShader;
        std::shared_ptr<VertexArray> m_squareVertexArray;

        OrthographicCamera camera;

    private:
        std::unique_ptr<Window> m_window;
        ImGuiLayer* m_imGuiLayer;
        bool m_running = true;
        LayerStack m_layerStack;

        static Application* s_instance;
    };

    //To be defined in client
    Application* CreateApplication();
}

