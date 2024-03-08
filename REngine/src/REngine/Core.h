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

// using this define, we can end up with a bit field for our event categories
// which allows events to be a part of multiple categories
#define BIT(x) (1 << x)