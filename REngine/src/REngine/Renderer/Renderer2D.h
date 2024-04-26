#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace REngine
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        // Draw primitives

        static void DrawQuad(const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, float rotationDeg, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, float rotationDeg, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec2& position, float rotationDeg, const glm::vec2& size, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, float rotationDeg, const glm::vec2& size, const Ref<Texture2D>& texture);

    };

}