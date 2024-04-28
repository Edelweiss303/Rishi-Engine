#include "REpch.h"
#include "Shader.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace REngine
{
    Ref<Shader> Shader::Create(const std::string& filepath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(filepath);
        default:
            RE_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
        }
    }

    Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        default:
            RE_CORE_ASSERT(false, "Unknown RendererAPI");
            return nullptr;
        }
    }

    void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
    {
        RE_CORE_ASSERT(!Exists(name), "Shader already exists!");
        m_shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader)
    {
        const std::string& name = shader->GetName();
        Add(name, shader);
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
    {
        auto shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const std::string& name)
    {
        RE_CORE_ASSERT(Exists(name), "Shader not found!");
        return m_shaders[name];
    }

    bool ShaderLibrary::Exists(const std::string& name) const
    {
        return m_shaders.find(name) != m_shaders.end();
    }
}