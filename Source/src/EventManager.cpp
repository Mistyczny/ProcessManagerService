#include "EventManager.hpp"

bool EventManager::initialize(std::multimap<uint32_t, std::unique_ptr<EventInterface>>& eventsMap) noexcept {
    bool eventManagerInitialized{true};
    if (!eventManager) {
        try {
            eventManager = new EventManager(eventsMap);
        } catch (std::bad_alloc& allocationError) {
            eventManagerInitialized = false;
        }
    }
    return eventManagerInitialized;
}

EventManager::EventManager(std::multimap<uint32_t, std::unique_ptr<EventInterface>>& eventsMap) : eventsMap{eventsMap} {}

EventManager::~EventManager() {
    if (eventManager) {
        delete eventManager;
    }
}
EventManager* EventManager::getEventManager() { return eventManager; }

void EventManager::registerNewEventHandler(uint32_t key, std::unique_ptr<EventInterface> handler) {
    auto eventManager = getEventManager();
    eventManager->eventsMap.emplace(key, std::move(handler));
}