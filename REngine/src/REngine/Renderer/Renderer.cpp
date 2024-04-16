#include "REpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace REngine
{
    Renderer::SceneData* Renderer::s_sceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_sceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_viewProjectionMatrix", s_sceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
}