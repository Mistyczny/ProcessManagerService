#include "EventManager.hpp"

bool EventManager::initialize(Service::MessageEventsCache& messageEventsCache,
                              Service::SubscriptionEventsCache& subscriptionEventsCache) noexcept {
    bool eventManagerInitialized{true};
    if (!eventManager) {
        try {
            eventManager = new EventManager(messageEventsCache, subscriptionEventsCache);
        } catch (std::bad_alloc& allocationError) {
            eventManagerInitialized = false;
        }
    }
    return eventManagerInitialized;
}

EventManager::EventManager(Service::MessageEventsCache& messageEventsCache, Service::SubscriptionEventsCache& subscriptionEventsCache)
    : messageEventsCache{messageEventsCache}, subscriptionEventsCache{subscriptionEventsCache} {}

EventManager::~EventManager() { delete eventManager; }
EventManager* EventManager::getEventManager() { return eventManager; }

std::optional<uint32_t> EventManager::registerNewEventHandler(uint32_t key, std::unique_ptr<EventInterface> eventHandler) {
    std::optional<uint32_t> eventID{std::nullopt};
    if (eventHandler) {
        try {
            eventID = eventManager->messageEventsCache.addMessageEvent(std::move(eventHandler));
        } catch (std::exception& ex) {
            Log::error("Failed to add message event to the cache with error" + std::string{ex.what()});
        }
    }
    return eventID;
}

std::optional<uint32_t> EventManager::registerNewSubscribtionHandler(std::unique_ptr<SubscribeEventInterface> subscriptionEvent) {
    std::optional<uint32_t> eventID{std::nullopt};
    if (subscriptionEvent) {
        try {
            eventID = eventManager->subscriptionEventsCache.addSubscriptionEvent(std::move(subscriptionEvent));
        } catch (std::exception& ex) {
            Log::error("Failed to add message event to the cache with error" + std::string{ex.what()});
        }
    }
    return eventID;
}