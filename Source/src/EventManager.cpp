#include "EventManager.hpp"

bool EventManager::initialize(Service::MessageEventsCache& messageEventsCache) noexcept {
    bool eventManagerInitialized{true};
    if (!eventManager) {
        try {
            eventManager = new EventManager(messageEventsCache);
        } catch (std::bad_alloc& allocationError) {
            eventManagerInitialized = false;
        }
    }
    return eventManagerInitialized;
}

EventManager::EventManager(Service::MessageEventsCache& messageEventsCache) : messageEventsCache{messageEventsCache} {}

EventManager::~EventManager() {
    if (eventManager) {
        delete eventManager;
    }
}
EventManager* EventManager::getEventManager() { return eventManager; }

std::optional<uint32_t> EventManager::registerNewEventHandler(uint32_t key, std::unique_ptr<EventInterface> eventHandler) {
    std::optional<uint32_t> eventID{std::nullopt};
    if (eventHandler) {
        auto eventManager = getEventManager();
        try {
            eventID = eventManager->messageEventsCache.addMessageEvent(std::move(eventHandler));
        } catch (std::exception& ex) {
            Log::error("Failed to add message event to the cache with error" + std::string{ex.what()});
        }
    }
    return eventID;
}