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

        /**
         * @brief Starts a timer for the specified duration. The callback will be called when the duration runs out.
         * 
         * @param id An unique identifier for the timer.
         * @param duration The duration of the timer in milliseconds.
         * @param callback The callback of the timer to be called at the end.
         */
        PX_API static void Start(CREFSTR id, float duration, const TimerCallback& callback);
        /**
         * @brief Starts a timer for the specified duration. The callback will be called every update until the duration runs out.
         * 
         * @param id An unique identifier for the timer.
         * @param duration The duration of the timer in milliseconds.
         * @param callback The callback of the timer to be called until the end.
         */
        PX_API static void StartTicked(CREFSTR id, float duration, const TickedTimerCallback& callback);
        /**
         * @brief Stops an active timer.
         * 
         * @param id The id of the timer.
         */
        PX_API static void Stop(CREFSTR id);
    };
};