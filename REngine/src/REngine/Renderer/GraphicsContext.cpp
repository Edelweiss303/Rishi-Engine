#include "REpch.h"
#include "GraphicsContext.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace REngine
{

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    
			RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
		case RendererAPI::API::OpenGL:  
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		default:
			RE_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}

}