#include "pixl/utility/Defs.h"

#include <iostream>
#include <cstdarg>
#include <string>
#include <memory>
#include <cstring>
#include <stdio.h>
#define _OPEN_SYS_ITOA_EXT
#include <stdlib.h>

using namespace px;

void __pixl_debug_log(const char* file, int line, const char* func, const char* fmt, ...)
{
    printf("[%s:%d] %s: ", file, line, func);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}
