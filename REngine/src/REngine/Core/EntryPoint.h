#pragma once

#ifdef RE_PLATFORM_WINDOWS

extern REngine::Application* REngine::CreateApplication();

int main(int argc, char** argv)
{
    REngine::Log::Init();
    RE_CORE_WARN("Initialized Log!");
    int r = 303;
    RE_INFO("Hello! Var={0}", r);

    auto app = REngine::CreateApplication();
    app->Run();
    delete app;
}

#endif