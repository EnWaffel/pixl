#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2i.h"
#include "pixl/core/input/KeyCode.h"

#define PX_EVENT(event) (px::Event)px::Events::event

namespace px
{
    typedef uint64_t Event;

    enum class Events : Event
    {
        KEY_PRESS = 1ULL << 0,
        KEY_RELEASE = 1ULL << 1,
        MOUSE_PRESS = 1ULL << 2,
        MOUSE_RELEASE = 1ULL << 3,
        MOUSE_MOVE = 1ULL << 4,
        WINDOW_CHANGED = 1ULL << 5,
        MOUSE_SCROLL = 1ULL << 6
    };

    struct EventData
    {
        Event type;
    };

    struct WindowChangedData : public EventData
    {
        Vec2i viewportPos;
        Vec2i viewportSize;
    };

    struct KeyEvent : public EventData
    {
        KeyCode code;
    };

    struct MouseEvent : public EventData
    {
        MouseButton button;
    };

    struct MouseMoveEvent : public EventData
    {
        float x;
        float y;
    };

    struct MouseScrollEvent : public EventData
    {
        float x;
        float y;
    };
}