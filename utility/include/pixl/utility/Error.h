#pragma once

#include "Defs.h"

#include <cstdint>

#define PX_NOERROR px::Error()
#define PX_SUCCESS 0

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
        PX_API Error(uint16_t code);

        PX_API explicit operator bool() const;
        PX_API operator uint16_t() const;
    private:
        bool m_HasError;
        uint16_t m_Code;
    public:
        PX_API static void Throw(uint16_t code, CREFSTR message);
        PX_API static bool HasError();
        PX_API static uint16_t GetErrorCode();
        PX_API static CREFSTR GetErrorMessage();
    };
}