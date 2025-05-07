#include "pixl/core/event/EventManager.h"

#include <unordered_map>

using namespace px;

namespace px
{
    struct ListenerData
    {
        EventListener listener;
        Event events;
        void* userData;
    };

    class EventManagerImpl
    {
    private:
        std::unordered_map<std::string, ListenerData> m_ListenersLOW;
        std::unordered_map<std::string, ListenerData> m_ListenersMEDIUM;
        std::unordered_map<std::string, ListenerData> m_ListenersHIGH;
    public:
        EventManagerImpl()
        {
        }

        ~EventManagerImpl()
        {
        }

        void AddListener(CREFSTR name, Event events, const EventListener& listener, ListenerPriority priority, void* userData)
        {
            if (m_ListenersLOW.count(name) > 0 || m_ListenersMEDIUM.count(name) > 0 || m_ListenersHIGH.count(name) > 0) return;

            switch (priority)
            {
            case ListenerPriority::LOW:
                m_ListenersLOW.insert({ name, { listener, events, userData } });
                break;
            case ListenerPriority::MEDIUM:
                m_ListenersMEDIUM.insert({ name, { listener, events, userData } });
                break;
            case ListenerPriority::HIGH:
                m_ListenersHIGH.insert({ name, { listener, events, userData } });
                break;
            }
        }

        void RemoveListener(CREFSTR name)
        {
            if (m_ListenersLOW.count(name) > 0) m_ListenersLOW.erase(name);
            if (m_ListenersMEDIUM.count(name) > 0) m_ListenersMEDIUM.erase(name);
            if (m_ListenersHIGH.count(name) > 0) m_ListenersHIGH.erase(name);
        }

        void CallEvent(Event id, const EventData& data)
        {
            if ((id & (id - 1)) != 0)
            {
                Error::Throw(PX_ERROR_INVALID_OPERATION, "Cannot call an event list");
                return;
            }

            for (const auto& v : m_ListenersHIGH)
            {
                if (v.second.events & id)
                {
                    if (!DoCallEvent(v.first, v.second, id, data)) return;
                }
            }

            for (const auto& v : m_ListenersMEDIUM)
            {
                if (v.second.events & id)
                {
                    if (!DoCallEvent(v.first, v.second, id, data)) return;
                }
            }

            for (const auto& v : m_ListenersLOW)
            {
                if (v.second.events & id)
                {
                    if (!DoCallEvent(v.first, v.second, id, data)) return;
                }
            }
        }

        bool DoCallEvent(CREFSTR name, const ListenerData& listener, Event id, const EventData& data)
        {
            try
            {
                return listener.listener(data, listener.userData);
            }
            catch (const std::exception& e)
            {
                Error::Throw(PX_ERROR_INVALID_OPERATION, "Caught exception in event handler (" + name + "): " + e.what());
            }

            return true;
        }
    };
};

px::EventManager::EventManager()
{
    m_Impl = new EventManagerImpl;
}

px::EventManager::~EventManager()
{
    delete m_Impl;
}

void px::EventManager::AddListener(CREFSTR name, Event events, const EventListener& listener, ListenerPriority priority, void* userData)
{
    m_Impl->AddListener(name, events, listener, priority, userData);
}

void px::EventManager::RemoveListener(CREFSTR name)
{
    m_Impl->RemoveListener(name);
}

void px::EventManager::CallEvent(Event id, const EventData& data)
{
    m_Impl->CallEvent(id, data);
}
