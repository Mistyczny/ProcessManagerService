#pragma once
#include "Event.hpp"
#include <map>

class EventManager {
private:
    static inline EventManager* eventManager{nullptr};
    std::multimap<uint32_t, std::unique_ptr<EventInterface>>& eventsMap;

public:
    explicit EventManager(std::multimap<uint32_t, std::unique_ptr<EventInterface>>& eventsMap);
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    virtual ~EventManager();
    static bool initialize(std::multimap<uint32_t, std::unique_ptr<EventInterface>>&) noexcept;

    static EventManager* getEventManager();

    static void registerNewEventHandler(uint32_t, std::unique_ptr<EventInterface>);
};