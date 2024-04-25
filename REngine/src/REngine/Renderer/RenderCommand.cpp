#include "REpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace REngine
{
    Scope<RendererAPI> RenderCommand::s_rendererAPI = CreateScope<OpenGLRendererAPI>();
}