#include "REpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

namespace REngine
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float TexIndex;
        float TilingFactor;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32; // TODO: Render Caps

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;

        uint32_t QuadIndexCount = 0;
        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 = white texture

        glm::vec4 QuadVertexPositions[4];

        Renderer2D::Statistics Stats;
    };


    static Renderer2DData s_data;


    void Renderer2D::Init()
    {
        RE_PROFILE_FUNCTION();

        uint32_t* quadIndices = new uint32_t[s_data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        s_data.QuadVertexArray = VertexArray::Create();
        s_data.QuadVertexBuffer = VertexBuffer::Create(s_data.MaxVertices * sizeof(QuadVertex));
        s_data.QuadVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float4, "a_color" },
            { ShaderDataType::Float2, "a_texCoord" },
            { ShaderDataType::Float, "a_texIndex" },
            { ShaderDataType::Float, "a_tilingFactor" }
            });

        s_data.QuadVertexArray->AddVertexBuffer(s_data.QuadVertexBuffer);
        s_data.QuadVertexBufferBase = new QuadVertex[s_data.MaxVertices];
        
        Ref<IndexBuffer> quadIndexBuffer = IndexBuffer::Create(quadIndices, s_data.MaxIndices);
        s_data.QuadVertexArray->SetIndexBuffer(quadIndexBuffer);
        delete[] quadIndices;
        
        
        uint32_t whiteTextureData = 0xffffffff;
        s_data.WhiteTexture = Texture2D::Create(1, 1);
        s_data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        int32_t samplers[s_data.MaxTextureSlots];
        for (int32_t i = 0; i < s_data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_data.TextureShader = Shader::Create("Assets/Shaders/Texture.glsl");
        s_data.TextureShader->Bind();
        s_data.TextureShader->SetIntArray("u_textures", samplers, s_data.MaxTextureSlots);

        s_data.TextureSlots[0] = s_data.WhiteTexture;

        s_data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
        s_data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::Shutdown()
    {
        RE_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        RE_PROFILE_FUNCTION();

        s_data.TextureShader->Bind();
        s_data.TextureShader->SetMat4("u_viewProjectionMatrix", camera.GetViewProjectionMatrix());

        s_data.QuadIndexCount = 0;
        s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

        s_data.TextureSlotIndex = 1;
    }

    void Renderer2D::EndScene()
    {
        RE_PROFILE_FUNCTION();

        // Cast to uint8_t so we get the number of elements in terms of bytes
        uint32_t dataSize = (uint8_t*)s_data.QuadVertexBufferPtr - (uint8_t*)s_data.QuadVertexBufferBase;
        s_data.QuadVertexBuffer->SetData(s_data.QuadVertexBufferBase, dataSize);
        Flush();
    }

    void Renderer2D::Flush()
    {
        // Bind all textures
        for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++)
        {
            s_data.TextureSlots[i]->Bind(i);
        }

        RenderCommand::DrawIndexed(s_data.QuadVertexArray, s_data.QuadIndexCount);
        s_data.Stats.DrawCalls++;
    }

    void Renderer2D::DrawQuad(const glm::vec4& color)
    {
        DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, color);
    }

    void Renderer2D::DrawQuad(const Ref<Texture2D>& texture)
    {
        DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        RE_PROFILE_FUNCTION();
        
        if (s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            FlushAndReset();
        }

        const float textureIndex = 0.0f; // White Texture;
        const float tilingFactor = 1.0f; 

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadIndexCount += 6;

        s_data.Stats.QuadCount++;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        RE_PROFILE_FUNCTION();

        if (s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            FlushAndReset();
        }

        float textureIndex = 0.0f;

        // Check if we've already submitted the texture in the texture slots
        for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
        {
            if (*s_data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_data.TextureSlotIndex;
            s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
            s_data.TextureSlotIndex++;
        }

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadIndexCount += 6;

        s_data.Stats.QuadCount++;

#if old_path
        s_data.TextureShader->SetFloat4("u_color", tintColor);
        s_data.TextureShader->SetFloat("u_tilingFactor", tilingFactor);
        texture->Bind();

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_data.TextureShader->SetMat4("u_transform", transform);


        s_data.QuadVertexArray->Bind();
        RenderCommand::DrawIndexed(s_data.QuadVertexArray);
#endif
    }

    void Renderer2D::DrawQuadRotated(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        DrawQuadRotated({ position.x, position.y, 0.0f }, rotation, size, color);
    }

    void Renderer2D::DrawQuadRotated(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
    {
        RE_PROFILE_FUNCTION();

        if (s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            FlushAndReset();
        }

        const float textureIndex = 0.0f; // White Texture;
        const float tilingFactor = 1.0f;

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
        s_data.QuadVertexBufferPtr->Color = color;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadIndexCount += 6;

        s_data.Stats.QuadCount++;
    }

    void Renderer2D::DrawQuadRotated(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        DrawQuadRotated({ position.x, position.y, 0.0f }, rotation, size, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuadRotated(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        RE_PROFILE_FUNCTION();

        if (s_data.QuadIndexCount >= Renderer2DData::MaxIndices)
        {
            FlushAndReset();
        }

        float textureIndex = 0.0f;

        // Check if we've already submitted the texture in the texture slots
        for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++)
        {
            if (*s_data.TextureSlots[i].get() == *texture.get())
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_data.TextureSlotIndex;
            s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
            s_data.TextureSlotIndex++;
        }

        glm::mat4 transform =
            glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[0];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[1];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[2];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadVertexBufferPtr->Position = transform * s_data.QuadVertexPositions[3];
        s_data.QuadVertexBufferPtr->Color = tintColor;
        s_data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_data.QuadVertexBufferPtr++;

        s_data.QuadIndexCount += 6;

        s_data.Stats.QuadCount++;
    }

    Renderer2D::Statistics Renderer2D::GetStats()
    {
        return s_data.Stats;
    }

    void Renderer2D::ResetStats()
    {
        memset(&s_data.Stats, 0, sizeof(Statistics));
    }

    void Renderer2D::FlushAndReset()
    {
        EndScene();

        s_data.QuadIndexCount = 0;
        s_data.QuadVertexBufferPtr = s_data.QuadVertexBufferBase;

        s_data.TextureSlotIndex = 1;
    }

}
