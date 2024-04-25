#include "REpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include <Platform/OpenGL/OpenGLShader.h>
#include "RenderCommand.h"

namespace REngine
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> FlatColorShader;
    };

    static Renderer2DStorage* s_data;

    void Renderer2D::Init()
    {
        s_data = new Renderer2DStorage();

        float squareVertices[5 * 3] =
        {
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f
        };

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

        s_data->QuadVertexArray = VertexArray::Create();

        Ref<VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        Ref<IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

        squareVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_position" }
            });

        s_data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);
        s_data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

        s_data->FlatColorShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
    }

    void Renderer2D::Shutdown()
    {
        delete s_data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->UploadUniformMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->UploadUniformMat4("u_transform", glm::mat4(1.0f));
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(s_data->FlatColorShader)->UploadUniformFloat4("u_color", color);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
    }

}
