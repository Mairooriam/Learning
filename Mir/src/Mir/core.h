#pragma once

#ifdef MIR_PLATFORM_WINDOWS
#if MIR_DYNAMIC_LINK
    #ifdef MIR_BUILD_DLL
        #define MIR_API __declspec(dllexport)
    #else
        #define MIR_API __declspec(dllimport)
    #endif
#else   
    #define MIR_API
#endif
#else
    #error Mir only supports Windows!
#endif

#ifdef MIR_ENABLE_ASSERTS
    #define MIR_ASSERT(x, ...) { if(!(x)) { MIR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MIR_CORE_ASSERT(x, ...) { if(!(x)) { MIR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else   
    #define MIR_ASSERT(x, ...)
    #define MIR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << (x))

#define MIR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)