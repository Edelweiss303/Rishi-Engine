#pragma once

#include "RendererAPI.h"

namespace REngine
{
    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_rendererAPI->Init();
        }

        inline static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_rendererAPI->SetViewPort(x, y, width, height);
        }

        inline static void SetClearColor(const glm::vec4& color) 
        { 
            s_rendererAPI->SetClearColor(color); 
        }

        inline static void Clear() 
        { 
            s_rendererAPI->Clear(); 
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
        { 
            s_rendererAPI->DrawIndexed(vertexArray, count); 
        }

    private:
        static Scope<RendererAPI> s_rendererAPI;
    };
}
