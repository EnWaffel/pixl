#include "pixl/core/pixl.h"

#include <GLFW/glfw3.h>
#include <vector>

using namespace px;

extern bool __pixl_error_log_errors;
#ifdef PX_VIDEO
extern void __pixl_vid_destroy_managed();
#endif

InitOptions __pixl_opts;
WINDOW __pixl_rootwnd = nullptr;
bool __pixl_singlewnd = true;
static std::vector<WINDOW> windows;

static int _CheckOpenGLVersion()
{
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* wnd = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (!wnd)
    {
        PX_DEBUG_LOG("_CheckOpenGLVersion()", "Failed to get OpenGL version! (This system may be very old or does not have any relevant drivers installed!)");
        return -1;
    }

    glfwMakeContextCurrent(wnd);

    int major = glfwGetWindowAttrib(wnd, GLFW_CONTEXT_VERSION_MAJOR);
    int minor = glfwGetWindowAttrib(wnd, GLFW_CONTEXT_VERSION_MINOR);

    PX_DEBUG_LOG("_CheckOpenGLVersion()", "OpenGL v%d.%d required <<->> OpenGL v%d.%d available", PX_OPENGL_VERSION_MAJOR, PX_OPENGL_VERSION_MINOR, major, minor);

    glfwDestroyWindow(wnd);

    return major + minor >= PX_OPENGL_VERSION_MAJOR + PX_OPENGL_VERSION_MINOR;
}

Error px::Init(const InitOptions& opts)
{
    __pixl_opts = opts;
    __pixl_error_log_errors = opts.errorLogging;

    PX_DEBUG_LOG("px::Init()", "Initializing...");

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

    int result = _CheckOpenGLVersion();
    if (result == 0)
    {
        PX_DEBUG_LOG("px::Init()", "Required OpenGL version not available!");

        char buf[128] = { 0 };
        snprintf(buf, sizeof(buf), "Required OpenGL version not available! (Required: >= %d.%d)", PX_OPENGL_VERSION_MAJOR, PX_OPENGL_VERSION_MINOR);

        Error::Throw(PX_ERROR_OPENGL, buf);
        return PX_ERROR_OPENGL;
    }
    else if (result == -1)
    {
        PX_DEBUG_LOG("px::Init()", "OpenGL not available!");

        char buf[128] = { 0 };
        snprintf(buf, sizeof(buf), "It seems as that OpenGL may not be available on this system. OpenGL is required to run this application! (version >= %d.%d)", PX_OPENGL_VERSION_MAJOR, PX_OPENGL_VERSION_MINOR);

        Error::Throw(PX_ERROR_OPENGL, buf);
        return PX_ERROR_OPENGL;
    }

    PX_DEBUG_LOG("px::Init()", "Init successful (GLFW version: %s)", glfwGetVersionString());

    return PX_NOERROR;
}

void px::End()
{
#ifdef PX_VIDEO
    __pixl_vid_destroy_managed();
#endif

    SoundSystem::End();
    AssetManager::End();

    for (WINDOW wnd : windows)
    {
        delete wnd;
    }
    
    glfwTerminate();
}

WINDOW px::CreateWindow(CREFSTR title, const Vec2i& size, bool startVisible, bool decorated)
{
    Error::Clear();

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
