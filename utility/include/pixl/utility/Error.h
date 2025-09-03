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
        /**
         * @brief Creates an Error instance with PX_SUCCESS.
         */
        PX_API Error();
        /**
         * @brief Creates an Error instance with the specified error code.
         * 
         * @param code The error code.
         */
        PX_API Error(uint64_t code);

        /**
         * @brief This operator overload makes it possible to use the Error class like this:
         * if (error) return;
         */
        PX_API explicit operator bool() const;
        /**
         * @brief Implicit or explicit conversion from an Error class instance to an uint64_t
         */
        PX_API operator uint64_t() const;
    private:
        bool m_HasError;
        uint64_t m_Code;
    public:
        /**
         * @brief Throws an error. May or may not open a message box depending on the InitOptions.errorLogging option.
         * 
         * @param code The error code.
         * @param message The error message.
         */
        PX_API static void Throw(uint64_t code, CREFSTR message);
        /**
         * @brief Returns true if an error has occurred or not been cleared.
         * 
         * @returns True if an error has occurred or not been cleared.
         */
        PX_API static bool HasError();
        /**
         * @brief Returns the last error code.
         * 
         * @returns An error code.
         */
        PX_API static uint64_t GetErrorCode();
        /**
         * @brief Returns the last error message.
         * 
         * @returns An error message.
         */
        PX_API static CREFSTR GetErrorMessage();
        /**
         * @brief Clears the last error.
         */
        PX_API static void Clear();
    };
}