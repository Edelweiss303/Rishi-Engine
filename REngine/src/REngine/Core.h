#pragma once

#ifdef RE_PLATFORM_WINDOWS
    #ifdef RE_BUILD_DLL
        #define RE_API __declspec(dllexport)
    #else
        #define RE_API __declspec(dllimport)
    #endif
#else
    #error Rishi Engine only supports Windows!
#endif

#ifdef RE_ENABLE_ASSERTS
    #define RE_ASSERT(x, ...) { if(!(x)) { RE_ERROR("Assertion Failed: {0}", __VA_ARFS__); __debugbreak(); } }
    #define RE_CORE_ASSERT(x, ...) { if(!(x)) { RE_CORE_ERROR("Assertion Failed: {0}", __VA_ARFS__); __debugbreak(); } }
#else
    #define RE_ASSERT(x, ...)
    #define RE_CORE_ASSERT(x, ...)
#endif

// using this define, we can end up with a bit field for our event categories
// which allows events to be a part of multiple categories
#define BIT(x) (1 << x)