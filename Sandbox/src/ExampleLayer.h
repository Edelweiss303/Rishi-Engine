#pragma once

#include "REngine.h"

class ExampleLayer : public REngine::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(REngine::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(REngine::Event& e) override;
private:
	REngine::ShaderLibrary m_ShaderLibrary;
	REngine::Ref<REngine::Shader> m_Shader;
	REngine::Ref<REngine::VertexArray> m_VertexArray;

	REngine::Ref<REngine::Shader> m_FlatColorShader;
	REngine::Ref<REngine::VertexArray> m_SquareVertexArray;

	REngine::Ref<REngine::Texture2D> m_Texture2D;
	REngine::Ref<REngine::Texture2D> m_LogoTexture2D;

	REngine::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};