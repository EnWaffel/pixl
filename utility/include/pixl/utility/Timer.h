#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

#include <functional>

namespace px
{
    typedef std::function<void()> TimerCallback;
    typedef std::function<void(float)> TickedTimerCallback;

    class Timer
    {
    public:
        PX_INTERNAL Timer() = delete;
        PX_INTERNAL ~Timer() = delete;

        PX_API static void Start(CREFSTR id, float duration, const TimerCallback& callback);
        PX_API static void StartTicked(CREFSTR id, float duration, const TickedTimerCallback& callback);
        PX_API static void Stop(CREFSTR id);
    };
};