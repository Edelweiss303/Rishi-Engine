#pragma once

#include "REngine/Core/Window.h"
#include "REngine/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace REngine
{
    class WindowsWindow : public Window
    {
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        unsigned int GetWidth() const override { return m_data.Width; }
        unsigned int GetHeight() const override { return m_data.Height; }

        // Window attributes
        void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        virtual void* GetNativeWindow() const { return m_window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_window;
        Scope<GraphicsContext> m_context;

        struct WindowData
        {
            std::string Title;
            unsigned int Width;
            unsigned int Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_data;

    };
}

