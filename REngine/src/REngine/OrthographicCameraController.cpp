#include "REpch.h"
#include "OrthographicCameraController.h"
#include "Input.h"
#include "Keycodes.h"

namespace REngine
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_rotation(rotation)
    {
    }
    void OrthographicCameraController::OnUpdate(TimeStep ts)
    {
        if (Input::IsKeyPressed(RE_KEY_A))
            m_cameraPosition.x -= m_cameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(RE_KEY_D))
            m_cameraPosition.x += m_cameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(RE_KEY_W))
            m_cameraPosition.y += m_cameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(RE_KEY_S))
            m_cameraPosition.y -= m_cameraTranslationSpeed * ts;

        if (m_rotation)
        {
            if (Input::IsKeyPressed(RE_KEY_Q))
                m_cameraRotation += m_cameraRotationSpeed * ts;
            if (Input::IsKeyPressed(RE_KEY_E))
                m_cameraRotation -= m_cameraRotationSpeed * ts;
        
            m_camera.SetRotation(m_cameraRotation);
        }

        m_camera.SetPosition(m_cameraPosition);

        m_cameraTranslationSpeed = m_zoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll));
        dispatcher.Dispatch<WindowResizeEvent>(RE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
    }

    bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& e)
    {
        m_zoomLevel -= e.GetYOffset() * 0.5f;
        m_zoomLevel = std::max(m_zoomLevel, 0.1f);
        m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
    {
        m_aspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        return false;
    }
}