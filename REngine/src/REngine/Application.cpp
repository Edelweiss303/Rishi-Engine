#include "REpch.h"
#include "Application.h"

#include "log.h"

#include "Renderer/Renderer.h"

namespace REngine
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_instance = nullptr;

    Application::Application()
    {
        RE_CORE_ASSERT(!s_instance, "Application already exists!")
        s_instance = this;
        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_imGuiLayer = new ImGuiLayer();
        PushOverlay(m_imGuiLayer);
        
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
        };

        uint32_t indices[3] = { 0, 1, 2 };

        float squareVertices[3 * 4] =
        {
           -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f,  0.75f, 0.0f,
           -0.75f,  0.75f, 0.0f
        };

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec4 a_color;

            out vec3 v_position;
            out vec4 v_color;

            void main()
            {
                v_position = a_position;
                v_color = a_color;
                gl_Position = vec4(a_position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec3 v_position;
            in vec4 v_color;

            void main()
            {
                color = vec4(v_position * 0.5 + 0.5, 1.0);
                color = v_color;
            }
        )";
        std::string blueShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_position;

            out vec3 v_position;

            void main()
            {
                v_position = a_position;
                gl_Position = vec4(a_position, 1.0);
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;
            
            in vec3 v_position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )"; 

        BufferLayout layout =
        {
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float4, "a_color" }
        };

        BufferLayout squareLayout =
        {
            { ShaderDataType::Float3, "a_position" }
        };

        m_vertexArray.reset(VertexArray::Create());

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        
        vertexBuffer->SetLayout(layout);
        
        m_vertexArray->AddVertexBuffer(vertexBuffer);
        m_vertexArray->SetIndexBuffer(indexBuffer);

        m_shader.reset(Shader::Create(vertexSrc, fragmentSrc));
        
        
        m_squareVertexArray.reset(VertexArray::Create()); 
        
        std::shared_ptr<VertexBuffer> squareVertexBuffer;
        squareVertexBuffer.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        std::shared_ptr<IndexBuffer> squareIndexBuffer;
        squareIndexBuffer.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        
        squareVertexBuffer->SetLayout(squareLayout);

        m_squareVertexArray->AddVertexBuffer(squareVertexBuffer);
        m_squareVertexArray->SetIndexBuffer(squareIndexBuffer);        
        
        m_blueShader.reset(Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_running)
        {
            RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
            RenderCommand::Clear();

            Renderer::BeginScene();

            m_blueShader->Bind();
            Renderer::Submit(m_squareVertexArray);

            m_shader->Bind();
            Renderer::Submit(m_vertexArray);

            Renderer::EndScene();

            for (Layer* layer : m_layerStack)
                layer->OnUpdate();

            m_imGuiLayer->Begin(); 

            for (Layer* layer : m_layerStack)
                layer->OnImGuiRender();

            m_imGuiLayer->End();
            m_window->OnUpdate();
        }
    }
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
}