#pragma once

#include "RendererAPI.h"

namespace REngine
{
    class RenderCommand
    {
    public:
        inline static void SetClearColor(const glm::vec4& color) 
        { 
            s_rendererAPI->SetClearColor(color); 
        }

        inline static void Clear() 
        { 
            s_rendererAPI->Clear(); 
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
        { 
            s_rendererAPI->DrawIndexed(vertexArray); 
        }

    private:
        static RendererAPI* s_rendererAPI;
    };
}
