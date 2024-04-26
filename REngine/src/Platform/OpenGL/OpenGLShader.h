#pragma once

#include "REngine/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO: REMOVE!!!

typedef unsigned int GLenum;

namespace REngine
{
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

        inline virtual const std::string& GetName() const override { return m_name; }

        virtual void UploadUniformInt(const std::string& name, const int& value) const;

        virtual void UploadUniformFloat(const std::string& name, const float& value) const;
        virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const;
        virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const;
        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const;

        virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        uint32_t m_rendererID;
        std::string m_name;
    };
}
