#pragma once

namespace REngine
{
    class VertexArray
    {
    public:
        virtual ~VertexArray() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Enable() const = 0;

        static VertexArray* Create(float* vertices, uint32_t size);
    };
}

