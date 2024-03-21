#include "repch.h"
#include "ImGuiLayer.h"

#include "REngine/KeyCodes.h"
#include "REngine/MouseButtonCodes.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "REngine/Application.h"

#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace REngine
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        
        // TEMPORARY: should eventually use custom keycodes
        io.KeyMap[ImGuiKey_Tab]          = RE_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow]    = RE_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow]   = RE_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow]      = RE_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow]    = RE_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp]       = RE_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown]     = RE_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home]         = RE_KEY_HOME;
        io.KeyMap[ImGuiKey_End]          = RE_KEY_END;
        io.KeyMap[ImGuiKey_Insert]       = RE_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete]       = RE_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace]    = RE_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space]        = RE_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter]        = RE_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape]       = RE_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A]            = RE_KEY_A;
        io.KeyMap[ImGuiKey_C]            = RE_KEY_C;
        io.KeyMap[ImGuiKey_V]            = RE_KEY_V;
        io.KeyMap[ImGuiKey_X]            = RE_KEY_X;
        io.KeyMap[ImGuiKey_Y]            = RE_KEY_Y;
        io.KeyMap[ImGuiKey_Z]            = RE_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::OnUpdate()
    {

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_time > 0.0 ? (time - m_time) : (1.0f / 60.0f);
        m_time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true; 
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseButtonPressedEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.MouseWheel += e.GetYOffset();
        io.MouseWheelH += e.GetXOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[RE_KEY_LEFT_CONTROL] || io.KeysDown[RE_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[RE_KEY_LEFT_SHIFT] || io.KeysDown[RE_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[RE_KEY_LEFT_ALT] || io.KeysDown[RE_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[RE_KEY_LEFT_SUPER] || io.KeysDown[RE_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());
        return false;
    }

}