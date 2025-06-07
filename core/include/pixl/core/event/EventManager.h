#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/event/Event.h"

#include <functional>

namespace px
{
    typedef std::function<bool(const EventData&, void*)> EventListener;

    enum class ListenerPriority : uint8_t
    {
        HIGH,
        MEDIUM,
        LOW
    };

    class EventManagerImpl;

    class EventManager
    {
    public:
        PX_INTERNAL EventManager();
        PX_INTERNAL ~EventManager();

        PX_API void AddListener(CREFSTR name, Event events, const EventListener& listener, ListenerPriority priority = ListenerPriority::LOW, void* userData = nullptr);
        PX_API void RemoveListener(CREFSTR name);
        PX_API void CallEvent(Event id, EventData& data);
    private:
        EventManagerImpl* m_Impl;
    };

    typedef EventManager* EVTMGR;
}