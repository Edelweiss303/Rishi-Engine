#include "REpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace REngine
{
    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_path(path)
    {
        // Upload image first
        int width;
        int height;
        int channels;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        RE_CORE_ASSERT(data, "Failed to load image!");

        m_width = width;
        m_height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;

        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        
        RE_CORE_ASSERT(internalFormat && dataFormat, "Format not supported!");

        // Then send data to the GPU
        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);
        
        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_rendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_rendererID);
    }
}