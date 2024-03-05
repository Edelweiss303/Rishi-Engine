#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern REngine::Application* REngine::CreateApplication();

int main(int argc, char** argv)
{
    auto app = REngine::CreateApplication();
    app->Run();
    delete app;
}

#endif