#pragma once

#include "REngine.h"

class Sandbox2D : public REngine::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(REngine::TimeStep ts);
    virtual void OnImGuiRender() override;
    void OnEvent(REngine::Event& e) override;

private:
    REngine::OrthographicCameraController m_cameraController;

    REngine::Ref<REngine::Texture2D> m_texture2D;
    REngine::Ref<REngine::Texture2D> m_logo;

    glm::vec4 m_squareColor = { 0.2f, 0.8f, 0.3f, 1.0f };

};

