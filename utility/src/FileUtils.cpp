#include "pixl/utility/FileUtils.h"
#include "pixl/utility/Defs.h"

#ifdef PX_WIN
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#elif define(PX_UNIX) PX_UNIX
#include <sys/stat.h>
#endif

#include <fstream>

using namespace px;

bool px::FileUtils::FileExists(CREFSTR path)
{
    std::ifstream stream(path, std::ios::out | std::ios::in);
    bool result = stream.is_open();
    stream.close();
    return result;
}

bool px::FileUtils::DirectoryExists(CREFSTR path)
{
#ifdef PX_WIN
    DWORD dwAttrib = GetFileAttributes(path.c_str());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && 
           (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#elif define(PX_UNIX)
    struct stat info;

    if (stat(path.c_str(), &info) != 0) {
        return false;
    }

    return (info.st_mode & S_IFDIR) != 0;
#else
#error Unsupported Operating System!
#endif
}
