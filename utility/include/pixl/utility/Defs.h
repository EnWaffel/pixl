#pragma once

#include <string>
#include <cstdint>
#include <iostream>

#define PX_VERSION { 1, 0 }

#if defined(_WIN32) || defined(_WIN64)
    #define PX_WIN
#elif defined(__unix__)
    #define PX_UNIX
#endif

#if defined(PX_WIN)
    #ifdef PXDLL_EXPORTS
        #define PX_API __declspec(dllexport)
    #else
        #define PX_API __declspec(dllimport)
    #endif
    #define PX_INTERNAL
#else
    #define PX_API __attribute__((visibility("default")))
    #define PX_INTERNAL __attribute__((visibility("hidden")))
#endif

#ifdef PX_DEBUG
#define PX_DEBUG_LOG(func, msg) std::cout << "[pixl::DEBUG] (" << __FILE__ << ":" << __LINE__ << "::" << func << ") " << msg << std::endl 
#else
#define PX_DEBUG_LOG
#endif

namespace px
{
    typedef const std::string& CREFSTR;
    struct VERSION
    {
        uint8_t minor;
        uint8_t major;
    };
};