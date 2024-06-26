#include "REpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace REngine
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        :m_windowHandle(windowHandle)
    {
        RE_CORE_ASSERT(windowHandle, "Window handle is null!");
    }

    void OpenGLContext::Init()
    {
        RE_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        RE_CORE_ASSERT(status, "Failed to initialize Glad!");

        RE_CORE_INFO("OpenGL Info:");
        RE_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
        RE_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
        RE_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

#ifdef RE_ENABLE_ASSERTS
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

        RE_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "REngine requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        RE_PROFILE_FUNCTION();

        glfwSwapBuffers(m_windowHandle);
    }
}