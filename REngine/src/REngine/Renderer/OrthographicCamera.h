#pragma once

#include <glm/glm.hpp>

namespace REngine
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        inline const glm::vec3& GetPosition() const { return position; }
        void SetPosition(const glm::vec3& pos);

        inline float GetRotation() const { return rotation; }
        void SetRotation(float rot);

        inline const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }
        inline const glm::mat4& GetViewMatrix() const { return viewMatrix; }
        inline const glm::mat4& GetViewProjectionMatrix() const { return viewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 projectionMatrix;
        glm::mat4 viewMatrix;
        glm::mat4 viewProjectionMatrix;

        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        float rotation = 0.0f;
    };
}
