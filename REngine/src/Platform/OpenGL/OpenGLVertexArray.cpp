#include "REpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace REngine
{
    OpenGLVertexArray::OpenGLVertexArray(float* vertices, uint32_t size)
    {
        glCreateVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);
    }

    
    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteProgram(m_vertexArray);
    }
    void OpenGLVertexArray::Enable() const
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    }
    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_vertexArray);
    }
    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
}