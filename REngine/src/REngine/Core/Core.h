#pragma once
#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
    #ifdef _WIN64
	    /* Windows x64  */
        #define RE_PLATFORM_WINDOWS
    #else
	    /* Windows x86 */
        #error "x86 Builds are not supported!"
    #endif

#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
     * so we must check all of them (in this order)
     * to ensure that we're running on MAC
     * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define RE_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define RE_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif
     /* We also have to check __ANDROID__ before __linux__
      * since android is based on the linux kernel
      * it has __linux__ defined */
#elif defined(__ANDROID__)
    #define RE_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define RE_PLATFORM_LINUX
    #error "Linux is not supported!"
#else
	    /* Unknown compiler/platform */
    #error "Unknown platform!"
#endif // End of platform detection


#ifdef RE_DEBUG#
    #if defined(RE_PLATFORM_WINDOWS)
        #define RE_DEBUGBREAK() __debugbreak()
    #elif defined(RE_PLATFORM_LINUX)
        #include <signal.h>
        #define RE_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    
    #define RE_ENABLE_ASSERTS
#endif 

#ifdef RE_ENABLE_ASSERTS
    #define RE_ASSERT(x, ...) { if(!(x)) { RE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RE_DEBUGBREAK(); } }
    #define RE_CORE_ASSERT(x, ...) { if(!(x)) { RE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); RE_DEBUGBREAK(); } }
#else
    #define RE_ASSERT(x, ...)
    #define RE_CORE_ASSERT(x, ...)
#endif

// using this define, we can end up with a bit field for our event categories
// which allows events to be a part of multiple categories
#define BIT(x) (1 << x)

#define RE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace REngine
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}