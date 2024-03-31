#include "REpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace REngine
{
    VertexArray* VertexArray::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLVertexArray(vertices, size);
            break;
        default:
            RE_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
            break;
        }
    }
}