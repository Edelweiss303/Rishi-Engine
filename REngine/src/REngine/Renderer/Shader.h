#pragma once

#include <string>
#include <glm/glm.hpp>

namespace REngine
{
    class Shader
    {
    public:
        virtual ~Shader() {};

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const = 0;
        virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const = 0;

        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);
    };
}