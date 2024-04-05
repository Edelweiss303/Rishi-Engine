#pragma once

#include <glm/glm.hpp>

namespace REngine
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);

        inline const glm::vec3& GetPosition() const { return m_position; }
        void SetPosition(const glm::vec3& position);

        inline float GetRotation() const { return m_rotation; }
        void SetRotation(float rotation);

        inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
        inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
        inline const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::mat4 m_viewProjectionMatrix;

        glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
        float m_rotation = 0.0f;
    };
}
