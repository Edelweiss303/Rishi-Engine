#include <REngine.h>
#include "imgui/imgui.h"
class ExampleLayer : public REngine::Layer
{
public:
    ExampleLayer()
        :Layer("Example")
    {
    }

    void OnUpdate() override
    {
    }

    void OnEvent(REngine::Event& event) override
    {
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("Hello, World!");
        ImGui::End();
    }
};

class Sandbox : public REngine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

REngine::Application* REngine::CreateApplication()
{
    return new Sandbox();
}