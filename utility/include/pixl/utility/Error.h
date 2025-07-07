#pragma once

#include "Defs.h"

#include <cstdint>

#define PX_NOERROR px::Error()
#define PX_SUCCESS 0
#define PX_NO_ERROR -1

#define PX_ERROR 0x90
#define PX_ERROR_INVALID_OPERATION 0x91
#define PX_ERROR_INTERNAL 0x92
#define PX_ERROR_LIB 0x93

namespace px
{
    class Error
    {
    public:
        PX_API Error();
        PX_API Error(uint64_t code);

        PX_API explicit operator bool() const;
        PX_API operator uint64_t() const;
    private:
        bool m_HasError;
        uint64_t m_Code;
    public:
        PX_API static void Throw(uint64_t code, CREFSTR message);
        PX_API static bool HasError();
        PX_API static uint64_t GetErrorCode();
        PX_API static CREFSTR GetErrorMessage();
        PX_API static void Clear();
    };
}