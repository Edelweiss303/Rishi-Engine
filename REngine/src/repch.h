 #pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "REngine/Core/Log.h"
#include "REngine/Debug/Instrumentor.h"

#ifdef RE_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#define RE_PROFILE 1
#if RE_PROFILE
    #define RE_PROFILE_BEGIN_SESSION(name, filepath) ::REngine::Instrumentor::Get().BeginSession(name, filepath)
    #define RE_PROFILE_END_SESSION() ::REngine::Instrumentor::Get().EndSession()
    #define RE_PROFILE_SCOPE(name) ::REngine::InstrumentationTimer timer##__LINE__(name);
    #define RE_PROFILE_FUNCTION() RE_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define RE_PROFILE_BEGIN_SESSION(name, filepath)
    #define RE_PROFILE_END_SESSION()
    #define RE_PROFILE_SCOPE(name)
    #define RE_PROFILE_FUNCTION()
#endif