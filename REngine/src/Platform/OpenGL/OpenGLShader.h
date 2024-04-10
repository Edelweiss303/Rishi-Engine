#pragma once

#include "REngine/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace REngine
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void UploadUniformInt(const std::string& name, const int& value) const;

        virtual void UploadUniformFloat(const std::string& name, const float& value) const;
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const;

        virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        uint32_t m_rendererID;
    };
}
