#include "REpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace REngine
{
    RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI;
}