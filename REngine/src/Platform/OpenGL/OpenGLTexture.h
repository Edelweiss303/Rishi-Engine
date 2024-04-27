#pragma once
#include "REngine/Renderer/Texture.h"
#include <glad/glad.h>

namespace REngine
{
    class OpenGLTexture2D : public Texture2D
    {
    public: 
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        inline virtual uint32_t GetWidth() const override { return m_width; }
        inline virtual uint32_t GetHeight() const override { return m_height; }

        virtual void SetData(void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot = 0) const override;

    private:
        std::string m_path;
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_rendererID;
        GLenum m_internalFormat;
        GLenum m_dataFormat;
    };
}