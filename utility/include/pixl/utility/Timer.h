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

        static void Start(CREFSTR id, float duration, const TimerCallback& callback);
        static void StartTicked(CREFSTR id, float duration, const TickedTimerCallback& callback);
        static void Stop(CREFSTR id);
    };
};