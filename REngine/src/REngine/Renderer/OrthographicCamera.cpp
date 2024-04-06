#include "repch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace REngine
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMatrix(1.0f)
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void OrthographicCamera::SetPosition(const glm::vec3& pos)
    {
        m_position = pos;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetRotation(float rot)
    {
        m_rotation = rot;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        m_viewMatrix = glm::inverse(
            glm::translate(glm::mat4(1.0f), m_position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1))
        );

        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }
}