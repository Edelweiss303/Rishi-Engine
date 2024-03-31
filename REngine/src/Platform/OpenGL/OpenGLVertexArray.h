#pragma once
#include "REngine/Renderer/VertexArray.h"

namespace REngine
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray(float* vertices, uint32_t size);
        virtual ~OpenGLVertexArray() override;

        virtual void Enable() const override;
        virtual void Bind() const override;
        virtual void Unbind() const override;

    private:
        unsigned int m_vertexArray;
    };
}

