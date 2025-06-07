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
#ifdef PX_VIDEO
    __pixl_vid_destroy_managed();
#endif

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

Vec2 px::GetMousePos(CAMERA cam, WINDOW wnd)
{
    if (!wnd) wnd = __pixl_rootwnd;
    if (!cam) cam = wnd->GetDefaultCamera();

    float zoom = cam->zoom;

    double xpos, ypos;
    glfwGetCursorPos((GLFWwindow*)wnd->GetHandle(), &xpos, &ypos);

    int width, height;
    glfwGetWindowSize((GLFWwindow*)wnd->GetHandle(), &width, &height);

    Vec2i viewportPos = wnd->GetViewportPos();
    Vec2i viewportSize = wnd->GetViewportSize();

    float mousePosInViewX = (float)((xpos - viewportPos.x) / (float)viewportSize.x);
    float mousePosInViewY = (float)((ypos - viewportPos.y) / (float)viewportSize.y);

    float mousePosInWindowX = mousePosInViewX * wnd->GetFixedSize().x;
    float mousePosInWindowY = mousePosInViewY * wnd->GetFixedSize().y;

    return Vec2(mousePosInWindowX, mousePosInWindowY);
}

bool px::IsMouseTouchingSprite(SPRITE sprite, CAMERA cam, WINDOW wnd)
{
    if (!sprite) return false;
    Vec2 mousePos = GetMousePos(cam, wnd);
    Vec2 diff = Vec2((sprite->size.x * sprite->scale.x) - sprite->size.x, (sprite->size.y * sprite->scale.y) - sprite->size.y);
    return Math::RectRectIntersection(sprite->pos - (diff / 2.0f), sprite->size + diff, mousePos, Vec2(1.0f, 1.0f));
}
