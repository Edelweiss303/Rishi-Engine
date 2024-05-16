#include "REpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace REngine
{
    static GLenum ShaderTypeFromString(const std::string& type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;

        RE_CORE_ASSERT(false, "Unknown shader type!");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        RE_PROFILE_FUNCTION();

        std::string source = ReadFile(filepath);
        auto shaderSources = PreProcess(source);
        Compile(shaderSources);

        // Parse through filepath for filename
        // for example: Assets/Shaders/Texture.glsl
        
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;

        m_name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_name(name)
    {
        RE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        Compile(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        RE_PROFILE_FUNCTION();

        glDeleteProgram(m_rendererID);
    }

    void OpenGLShader::Bind() const
    {
        RE_PROFILE_FUNCTION();

        glUseProgram(m_rendererID);
    }

    void OpenGLShader::Unbind() const
    {
        RE_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        RE_PROFILE_FUNCTION();

        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
    {
        RE_PROFILE_FUNCTION();
        UploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        RE_PROFILE_FUNCTION();

        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        RE_PROFILE_FUNCTION();

        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        RE_PROFILE_FUNCTION();

        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        RE_PROFILE_FUNCTION();

        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vector) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform2f(location, vector.x, vector.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vector) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vector) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
    {
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
    {
        // Upload our mat4 into our shaders
        GLint location = glGetUniformLocation(m_rendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        RE_PROFILE_FUNCTION();

        std::string result;

        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
                in.close();
            }
            else
            {
                RE_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        }
        else
        {
            RE_CORE_ERROR("Could not open file at {0}", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        RE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            RE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            RE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            RE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

            pos = source.find(typeToken, nextLinePos);

            shaderSources[ShaderTypeFromString(type)] = 
                (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        RE_PROFILE_FUNCTION();

        // Get program object
        GLuint program = glCreateProgram();

        RE_CORE_ASSERT(shaderSources.size() <= 2, "Only 2 shaders supported for now.");

        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;

        for (auto& pair : shaderSources)
        {
            GLenum type = pair.first;
            const std::string& source = pair.second;

            // Create an empty shader handle
            GLuint shader = glCreateShader(type);

            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            // Compile the vertex shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                RE_CORE_ERROR("{0}", infoLog.data());
                RE_CORE_ASSERT(false, "Shader compilation failure.");
                break;
            }

            // Attach our shaders to our program
            glAttachShader(program, shader);

            // Keep track of shader IDs
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.

        // Link our program
        glLinkProgram(program);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);

            // Don't leak shaders either.

            for (GLenum id : glShaderIDs)
            {
                glDeleteShader(id);
            }

            RE_CORE_ERROR("{0}", infoLog.data());
            RE_CORE_ASSERT(false, "Shader link failure.");
            return;
        }

        // Always detach shaders after a successful link.
        for (GLenum id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_rendererID = program;
    }
}