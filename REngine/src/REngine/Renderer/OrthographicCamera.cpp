#include "repch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace REngine
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), viewMatrix(1.0f)
    {
        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }

    void OrthographicCamera::SetPosition(const glm::vec3& pos)
    {
        position = pos;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetRotation(float rot)
    {
        rotation = rot;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        viewMatrix = glm::inverse(
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(-rotation), glm::vec3(0, 0, 1))
        );

        viewProjectionMatrix = projectionMatrix * viewMatrix;
    }
}