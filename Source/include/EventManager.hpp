#pragma once
#include "Event.hpp"
#include "ServiceMessageEventsCache.hpp"
#include "ServiceSubscriptionEventsCache.hpp"
#include <map>
#include <optional>

class EventManager {
private:
    static inline EventManager* eventManager{nullptr};
    Service::MessageEventsCache& messageEventsCache;
    Service::SubscriptionEventsCache& subscriptionEventsCache;

public:
    explicit EventManager(Service::MessageEventsCache& messageEventsCache, Service::SubscriptionEventsCache& subscriptionEventsCache);
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
    virtual ~EventManager();
    static bool initialize(Service::MessageEventsCache& messageEventsCache,
                           Service::SubscriptionEventsCache& subscriptionEventsCache) noexcept;

    static EventManager* getEventManager();

    /***
     *
     * @param id
     * @param messageEvent
     * @return eventHandler
     */
    static std::optional<uint32_t> registerNewEventHandler(uint32_t id, std::unique_ptr<EventInterface> messageEvent);

    /***
     *
     * @param subscriptionEvent
     * @return subscriptionEventHandler
     */
    static std::optional<uint32_t> registerNewSubscribtionHandler(std::unique_ptr<SubscribeEventInterface> subscriptionEvent);


};