#pragma once
#include "REngine/Renderer/Shader.h"

namespace REngine
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const override;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const override;

    private:
        uint32_t m_rendererID;
    };
}