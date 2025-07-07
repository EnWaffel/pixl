#include "pixl/utility/Error.h"

#include <iostream>
#include <sstream>
#ifdef PX_WIN
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#endif

bool __pixl_error_log_errors = false;

using namespace px;

Error::Error() : m_HasError(false)
{
}

Error::Error(uint64_t code) : m_HasError(code != PX_SUCCESS), m_Code(code)
{
}

px::Error::operator bool() const
{
    return !m_HasError;
}

px::Error::operator uint64_t() const
{
    return m_Code;
}

static uint16_t __pixl__cur_error_code = 0;
static std::string __pixl__cur_error_message;
static bool __init = false;
static std::string __pixl_error_popup_program;

#ifndef PX_WIN
static bool _has_program(CREFSTR name)
{
#ifdef PX_UNIX
    return system(("which " + name + " > /dev/null 2>&1").c_str()) == 0;
#elif defined(PIX_WIN)
    return false;
#else
    return false;
#endif
}
#endif

static void _init()
{
    if (__init) return;
    __init = true;

#ifndef PX_WIN    
    if (_has_program("zenity"))
    {
        __pixl_error_popup_program = "zenity";
    }
    else if (_has_program("kdialog"))
    {
        __pixl_error_popup_program = "kdialog";
    }
#else
    __pixl_error_popup_program = "win";
#endif

}

static void _show_error_popup(CREFSTR message)
{
    if (__pixl_error_popup_program == "zenity" || __pixl_error_popup_program == "kdialog")
    {
        system((__pixl_error_popup_program + " --error --title=\"pixl Error\" --text=\"" + message + "\"").c_str());
    }
    else if (__pixl_error_popup_program == "win")
    {
#ifdef PX_WIN
#ifdef __MINGW32__
    MessageBox(NULL, message.c_str(), "pixl Error", MB_ICONERROR | MB_OK);
#else
    std::wstring str(message.begin(), message.end());
    MessageBox(NULL, str.c_str(), L"pixl Error", MB_ICONERROR | MB_OK);
#endif
#endif
    }
}

void px::Error::Throw(uint64_t code, CREFSTR message)
{
    if (!__init) _init();

    __pixl__cur_error_code = code;
    __pixl__cur_error_message = message;

    if (__pixl_error_log_errors)
    {
        std::cout << "[pixl::Error] 0x"
        << std::hex << code
        << ": "
        << message
        << ""
        << std::endl;
    }
    else
    {
        std::stringstream s;
        s << "0x"
        << std::hex << code
        << ": "
        << message;
        _show_error_popup(s.str());
    }
}

bool px::Error::HasError()
{
    return __pixl__cur_error_code != PX_SUCCESS;
}

uint64_t px::Error::GetErrorCode()
{
    return __pixl__cur_error_code;
}

CREFSTR px::Error::GetErrorMessage()
{
    return __pixl__cur_error_message;
}

void px::Error::Clear()
{
    __pixl__cur_error_code = PX_SUCCESS;
    __pixl__cur_error_message.clear();
}
