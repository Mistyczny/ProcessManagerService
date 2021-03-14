#pragma once
#include "Event.hpp"
#include "ServiceMessageEventsCache.hpp"
#include <map>
#include <optional>

class EventManager {
private:
    static inline EventManager* eventManager{nullptr};
    Service::MessageEventsCache& messageEventsCache;

public:
    explicit EventManager(Service::MessageEventsCache& messageEventsCache);
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    virtual ~EventManager();
    static bool initialize(Service::MessageEventsCache& messageEventsCache) noexcept;

    static EventManager* getEventManager();

    ///
    static std::optional<uint32_t> registerNewEventHandler(uint32_t id, std::unique_ptr<EventInterface>);
};