#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2i.h"

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
        WINDOW_CHANGED = 1ULL << 5
    };

    struct EventData
    {
    };

    struct WindowChangedData : public EventData
    {
        Vec2i viewportPos;
        Vec2i viewportSize;
    };
}