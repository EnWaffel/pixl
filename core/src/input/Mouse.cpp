#include "pixl/core/input/Mouse.h"
#include "pixl/core/window/Window.h"
#include "pixl/core/graphics/2d/Sprite.h"
#include "pixl/core/math/MathFunctions.h"

#include <glfw/glfw3.h>
#include <unordered_map>

using namespace px;

ButtonEntry Mouse::BUTTON1 = {};
ButtonEntry Mouse::BUTTON2 = {};
ButtonEntry Mouse::BUTTON3 = {};
ButtonEntry Mouse::BUTTON4 = {};
ButtonEntry Mouse::BUTTON5 = {};
ButtonEntry Mouse::BUTTON6 = {};
ButtonEntry Mouse::BUTTON7 = {};
ButtonEntry Mouse::BUTTON8 = {};
ButtonEntry Mouse::BUTTON_LAST = {};
ButtonEntry Mouse::BUTTON_LEFT = {};
ButtonEntry Mouse::BUTTON_RIGHT = {};
ButtonEntry Mouse::BUTTON_MIDDLE = {};

extern WINDOW __pixl_rootwnd;

static std::unordered_map<MouseButton, ButtonEntry*> entries;

static bool OnButtonPress(const EventData& data, void* ignored)
{
    const MouseEvent& event = (const MouseEvent&)data;

    if (entries.count(event.button) < 1) return true;

    entries.at(event.button)->justPressed = true;
    entries.at(event.button)->pressed = true;

    return true;
}

static bool OnButtonRelease(const EventData& data, void* ignored)
{
    const MouseEvent& event = (const MouseEvent&)data;

    if (entries.count(event.button) < 1) return true;

    entries.at(event.button)->justReleased = true;
    entries.at(event.button)->pressed = false;

    return true;
}

void px::Mouse::Init(Window* rootWnd)
{
    rootWnd->GetEventManager()->AddListener("__pixl_buttons_press", PX_EVENT(MOUSE_PRESS), OnButtonPress, ListenerPriority::HIGH);
    rootWnd->GetEventManager()->AddListener("__pixl_buttons_release", PX_EVENT(MOUSE_RELEASE), OnButtonRelease, ListenerPriority::HIGH);

    entries.insert({  });
}

void px::Mouse::Update()
{
    for (auto& v : entries)
    {
        v.second->justPressed = false;
        v.second->justReleased = false;
    }
}

Vec2 px::Mouse::GetPosition(Camera* cam, Window* wnd)
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

bool px::Mouse::TouchingSprite(Sprite* sprite, Camera* cam, Window* wnd)
{
    if (!sprite) return false;
    Vec2 mousePos = GetPosition(cam, wnd);
    Vec2 diff = Vec2((sprite->size.x * sprite->scale.x) - sprite->size.x, (sprite->size.y * sprite->scale.y) - sprite->size.y);
    return Math::RectRectIntersection(sprite->pos - (diff / 2.0f), sprite->size + diff, mousePos, Vec2(1.0f, 1.0f));
}
