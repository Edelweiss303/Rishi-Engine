#include "REpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace REngine
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
            break;
        default:
            RE_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
            break;
        }
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
            break;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, size);
            break;
        default:
            RE_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
            break;
        }
    }
}