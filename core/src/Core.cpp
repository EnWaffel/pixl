#include "pixl/core/pixl.h"

#include <GLFW/glfw3.h>
#include <vector>

using namespace px;

extern bool __pixl_error_log_errors;

InitOptions __pixl_opts;
WINDOW __pixl_rootwnd = nullptr;
bool __pixl_singlewnd = true;
static std::vector<WINDOW> windows;

Error px::Init(const InitOptions& opts)
{
    __pixl_opts = opts;
    if (opts.errorLogging)
    {
        __pixl_error_log_errors = true;
    }

    if (glfwInit() != GLFW_TRUE)
    {
        Error::Throw(PX_ERROR_GLFW, "Failed to initialize GLFW");
        return PX_ERROR_GLFW;
    }

    if (!AssetManager::Init())
    {
        Error::Throw(PX_ERROR_SERVICE, "Failed to initialize the Asset Manager");
        return PX_ERROR_SERVICE;
    }

    if (!SoundSystem::Init())
    {
        Error::Throw(PX_ERROR_SERVICE, "Failed to initialize the Sound System");
        return PX_ERROR_SERVICE;
    }

    PX_DEBUG_LOG("px::Init()", "Init successful (GLFW version: %s)", glfwGetVersionString());

    return PX_NOERROR;
}

void px::End()
{
    AssetManager::End();

    for (WINDOW wnd : windows)
    {
        delete wnd;
    }
    
    glfwTerminate();
}

WINDOW px::CreateWindow(CREFSTR title, const Vec2i& size, bool startVisible, bool decorated)
{
    WINDOW wnd = new Window(title, size, startVisible, decorated);
    wnd->PostInit();

    if (Error::HasError())
    {
        delete wnd;
        return nullptr;
    }

    windows.push_back(wnd);

    return wnd;
}

float px::GetTime()
{
    return glfwGetTime();
}
