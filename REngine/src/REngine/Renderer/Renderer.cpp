#include "REpch.h"
#include "Renderer.h"

namespace REngine
{
    Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& cam)
    {
        sceneData->ViewProjectionMatrix = cam.GetViewProjectionMatrix();
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();
        shader->UploadUniformMat4("viewProjectionMatrix", sceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer::EndScene()
    {
    }
}