#include "REpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

namespace REngine
{
    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage* s_data;

    void Renderer2D::Init()
    {
        s_data = new Renderer2DStorage();

        float squareVertices[5 * 4] =
        {
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 
           -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

        s_data->QuadVertexArray = VertexArray::Create();

        Ref<VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        Ref<IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

        squareVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float2, "a_texCoord" }
            });

        s_data->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);
        s_data->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
        
        uint32_t whiteTextureData = 0xffffffff;
        s_data->WhiteTexture = Texture2D::Create(1, 1);
        s_data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_data->TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
        s_data->TextureShader->Bind();
        s_data->TextureShader->SetInt("u_texture", 0);
    }

    void Renderer2D::Shutdown()
    {
        delete s_data;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_data->TextureShader->Bind();
        s_data->TextureShader->SetMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec4& color)
    {
        DrawQuad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 0.0f, 0.0f }, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, float rotationDeg, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, rotationDeg, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, float rotationDeg, const glm::vec2& size, const glm::vec4& color)
    {
        s_data->TextureShader->SetFloat4("u_color", color);
        s_data->WhiteTexture->Bind();

        glm::mat4 transform = 
            glm::translate(glm::mat4(1.0f), position) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        s_data->TextureShader->SetMat4("u_transform", transform);

        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(const Ref<Texture2D>& texture)
    {
        DrawQuad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, float rotationDeg, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        DrawQuad({ position.x, position.y, 0.0f }, rotationDeg, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, float rotationDeg, const glm::vec2& size, const Ref<Texture2D>& texture)
    {
        s_data->TextureShader->SetFloat4("u_color", glm::vec4(1.0f));
        texture->Bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_data->TextureShader->SetMat4("u_transform", transform);


        s_data->QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data->QuadVertexArray);
    }

}
