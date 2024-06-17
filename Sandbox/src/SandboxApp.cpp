#include <REngine.h>
#include <REngine/Core/EntryPoint.h>
#include "Sandbox2D.h"
#include "ExampleLayer.h"


class Sandbox : public REngine::Application
{
public:
    Sandbox()
    {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {

    }
};

REngine::Application* REngine::CreateApplication()
{
    return new Sandbox();
}