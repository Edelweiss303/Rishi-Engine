#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace REngine
{
    class RendererAPI
    {
    public:
        // class enums are safer because they
        // don't implicitly convert to other types
        enum class API
        {
            None = 0,
            OpenGL = 1
        };

    public:
        virtual void Clear() = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_API; }
    private:
        static API s_API;
    };
}

