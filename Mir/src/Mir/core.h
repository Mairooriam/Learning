#pragma once

#ifdef MIR_PLATFORM_WINDOWS
    #ifdef MIR_BUILD_DLL
        #define MIR_API __declspec(dllexport)
    #else
        #define MIR_API __declspec(dllimport)
    #endif
#else
    #error Mir only supports Windows!
#endif

#define BIT(x) (1 << (x))