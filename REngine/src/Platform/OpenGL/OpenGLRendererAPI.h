#pragma once

#include "REngine/Renderer/RendererAPI.h"

namespace REngine
{
    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Clear() override;
        virtual void SetClearColor(const glm::vec4& color) override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
}
