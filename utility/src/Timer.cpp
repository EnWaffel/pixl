#include "pixl/utility/Timer.h"

#include <unordered_map>
#include <queue>

using namespace px;

struct TimerData
{
    float duration;
    float waited;
    TimerCallback callback;
};

struct TickedTimerData
{
    float duration;
    float waited;
    TickedTimerCallback callback;
};

static std::unordered_map<std::string, TimerData> __timers;
static std::unordered_map<std::string, TickedTimerData> __ticked_timers;
static std::queue<std::pair<std::string, TimerData>> __timer_queue;
static std::queue<std::pair<std::string, TickedTimerData>> __ticked_timer_queue;
static std::queue<std::string> __timer_erase_queue;
static std::queue<std::string> __ticked_timer_erase_queue;

void __pixl_timer_update(float delta)
{
    while (!__timer_erase_queue.empty())
    {
        __timers.erase(__timer_erase_queue.front());
        __timer_erase_queue.pop();
    }

    while (!__ticked_timer_erase_queue.empty())
    {
        __ticked_timers.erase(__ticked_timer_erase_queue.front());
        __ticked_timer_erase_queue.pop();
    }

    while (!__timer_queue.empty())
    {
        __timers.insert(__timer_queue.front());
        __timer_queue.pop();
    }

    while (!__ticked_timer_queue.empty())
    {
        __ticked_timers.insert(__ticked_timer_queue.front());
        __ticked_timer_queue.pop();
    }

    for (auto& v : __timers)
    {
        v.second.waited += delta;
        if (v.second.waited >= v.second.duration)
        {
            Timer::Stop(v.first);
            v.second.callback();
        }
    }

    for (auto& v : __ticked_timers)
    {
        v.second.waited += delta;
        if (v.second.waited >= v.second.duration)
        {
            Timer::Stop(v.first);
        }
        else
        {
            v.second.callback(delta);
        }
    }
}

void px::Timer::Start(CREFSTR id, float duration, const TimerCallback& callback)
{
    if (__timers.count(id) > 0) return;

    TimerData data{};
    data.duration = duration;
    data.waited = 0.0f;
    data.callback = callback;

    __timer_queue.push(std::make_pair(id, data));
}

void px::Timer::StartTicked(CREFSTR id, float duration, const TickedTimerCallback& callback)
{
    if (__ticked_timers.count(id) > 0) return;

    TickedTimerData data{};
    data.duration = duration;
    data.waited = 0.0f;
    data.callback = callback;

    __ticked_timer_queue.push(std::make_pair(id, data));
}

void px::Timer::Stop(CREFSTR id)
{
    if (__timers.count(id) > 0) __timer_erase_queue.push(id);
    if (__ticked_timers.count(id) > 0) __ticked_timer_erase_queue.push(id);
}
