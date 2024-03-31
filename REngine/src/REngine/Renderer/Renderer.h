#pragma once

namespace REngine
{
    // class enums are safer because they
    // don't implicitly convert to other types
    enum class RendererAPI
    {
        None = 0,
        OpenGL = 1
    };

    class Renderer
    {
    public:
        inline static RendererAPI GetAPI() { return s_rendererAPI; }

    private:
        static RendererAPI s_rendererAPI;
    };
    
}