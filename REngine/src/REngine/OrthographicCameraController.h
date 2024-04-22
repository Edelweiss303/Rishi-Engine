#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Core/TimeStep.h"

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace REngine
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false); //aspect ratio * 2 units

        void OnUpdate(TimeStep ts);
        void OnEvent(Event& e);

        inline OrthographicCamera& GetCamera() { return m_camera; }
        inline const OrthographicCamera& GetCamera() const { return m_camera; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);

    private:
        float m_aspectRatio;
        float m_zoomLevel = 1.0f;
        OrthographicCamera m_camera;

        bool m_rotation;

        glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
        float m_cameraRotation = 0.0f;

        float m_cameraTranslationSpeed = 1.0f;
        float m_cameraRotationSpeed = 25.0f;
    };
}

