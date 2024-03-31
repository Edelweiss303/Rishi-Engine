#include "repch.h"
#include "Application.h"

#include "REngine/log.h"
#include "glad/glad.h"

namespace REngine
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_instance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return GL_FLOAT;
        case ShaderDataType::Float2:  return GL_FLOAT;
        case ShaderDataType::Float3:  return GL_FLOAT;
        case ShaderDataType::Float4:  return GL_FLOAT;
        case ShaderDataType::Mat3:    return GL_FLOAT;
        case ShaderDataType::Mat4:    return GL_FLOAT;
        case ShaderDataType::Int:     return GL_INT;
        case ShaderDataType::Int2:    return GL_INT;
        case ShaderDataType::Int3:    return GL_INT;
        case ShaderDataType::Int4:    return GL_INT;
        case ShaderDataType::Bool:    return GL_BOOL;
        }

        RE_CORE_ASSERT(false, "Unknown ShaderDatatype.");
        return 0;
    }

    Application::Application()
    {
        RE_CORE_ASSERT(!s_instance, "Application already exists!")
        s_instance = this;
        m_window = std::unique_ptr<Window>(Window::Create());
        m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        
        float vertices[3 * 7] =
        {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, 
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
        };

        uint32_t indices[3] = { 0, 1, 2 };

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

        m_vertexArray.reset(VertexArray::Create(vertices, sizeof(vertices)));
        
        m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        {
            BufferLayout layout =
            {
                { ShaderDataType::Float3, "a_position" },
                { ShaderDataType::Float4, "a_color" }
            };

            m_vertexBuffer->SetLayout(layout);
        }

        uint32_t index = 0;
        const auto& layout = m_vertexBuffer->GetLayout();

        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index, 
                element.GetComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(element.Type), 
                element.Normalized ? GL_TRUE: GL_FALSE, 
                layout.GetStride(),
                (const void*)element.Offset
            );

            index++;
        }

        m_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_shader.reset(Shader::Create(vertexSrc, fragmentSrc));
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_running)
        {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->Bind();
            m_vertexArray->Bind();

            glDrawElements(GL_TRIANGLES, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_layerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin(); 

            for (Layer* layer : m_layerStack)
                layer->OnImGuiRender();

            m_ImGuiLayer->End();
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