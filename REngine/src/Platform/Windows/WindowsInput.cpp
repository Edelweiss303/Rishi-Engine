#include "REpch.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>
#include "REngine/Core/Application.h"

namespace REngine
{
    Scope<Input> Input::s_instance = CreateScope<WindowsInput>();

    bool REngine::WindowsInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        auto state = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos;
        double yPos;
        glfwGetCursorPos(window, &xPos, &yPos);

        return std::pair<float, float>(xPos, yPos);
    }
}