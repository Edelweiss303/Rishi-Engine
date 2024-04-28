#pragma once
#include "REngine/Core/Core.h"

#ifdef RE_PLATFORM_WINDOWS

extern REngine::Application* REngine::CreateApplication();

int main(int argc, char** argv)
{
    REngine::Log::Init();

    RE_PROFILE_BEGIN_SESSION("Startup", "REngineProfile-Startup.json");
    auto app = REngine::CreateApplication();
    RE_PROFILE_END_SESSION();

    RE_PROFILE_BEGIN_SESSION("Runtime", "REngineProfile-Runtime.json");
    app->Run();
    RE_PROFILE_END_SESSION();

    RE_PROFILE_BEGIN_SESSION("Shutdown", "REngineProfile-Shutdown.json");
    delete app;
    RE_PROFILE_END_SESSION();
}

#endif