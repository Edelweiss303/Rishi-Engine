#include "REpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace REngine
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::API::OpenGL:
            return new OpenGLVertexArray();
            break;
        default:
            RE_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
            break;
        }
    }
}