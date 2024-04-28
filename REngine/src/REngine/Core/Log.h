#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace REngine
{
    class Log
    {
    public:
        static void Init();

        inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define RE_CORE_TRACE(...)   ::REngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)    ::REngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)    ::REngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...)   ::REngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_FATAL(...)   ::REngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define RE_TRACE(...)        ::REngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)         ::REngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...)         ::REngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...)        ::REngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define RE_FATAL(...)        ::REngine::Log::GetClientLogger()->fatal(__VA_ARGS__)