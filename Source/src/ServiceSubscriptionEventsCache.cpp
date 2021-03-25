/**
 * @file ServiceSubscriptionEventsCache.hpp
 * @brief Service subscriptions events cache
 * @author Kacper Waśniowski
 * @copyright Kacper Waśniowski, All Rights Reserved
 * @date 2021
 */
#include "ServiceSubscriptionEventsCache.hpp"

namespace Service {

uint32_t SubscriptionEventsCache::getNewEventID() const {
    uint32_t availableEventID{};
    for (auto& iter : events) {
        if (iter.id == availableEventID) {
            availableEventID++;
        } else {
            Log::trace("Found empty subscription event id = " + std::to_string(availableEventID));
            break;
        }
    }
    return availableEventID;
}

uint32_t SubscriptionEventsCache::addSubscriptionEvent(std::unique_ptr<SubscribeEventInterface> newEvent) {
    uint32_t subscriptionEventID{};
    std::unique_lock uniqueLock{sharedMutex};
    if (newEvent) {
        subscriptionEventID = this->getNewEventID();
        Subscription newSubscription{};
        newSubscription.id = subscriptionEventID;
        newSubscription.event = std::move(newEvent);
        events.emplace_back(std::move(newSubscription));
    } else {
        throw std::runtime_error("Trying to add null message event");
    }
    return subscriptionEventID;
}

std::vector<std::pair<Types::ModuleIdentifier, google::protobuf::Any>>
SubscriptionEventsCache::triggerMessageHandlers(google::protobuf::Any& anyMessage) {
    std::shared_lock sharedLock{sharedMutex};
    std::vector<std::pair<Types::ModuleIdentifier, google::protobuf::Any>> serviceRequests{};
    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (event.messageType == anyMessage.type_url()) {
            std::for_each(std::begin(event.subscribers), std::end(event.subscribers), [&](auto& subscriber) {
                auto newMessage = event.event->subscriptionHandle(anyMessage);
                if (newMessage.has_value()) {
                    serviceRequests.push_back(std::make_pair(subscriber, std::move(*newMessage)));
                }
            });
        }
    });
    return serviceRequests;
}

size_t SubscriptionEventsCache::size() const {
    std::shared_lock sharedLock{sharedMutex};
    return this->events.size();
}

void SubscriptionEventsCache::addSubscriber(Types::ModuleIdentifier moduleIdentifier, std::string& typeUrl) {
    std::unique_lock uniqueLock{sharedMutex};
    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (event.messageType == typeUrl) {
            event.subscribers.emplace_back(moduleIdentifier);
        }
    });
}

void SubscriptionEventsCache::addSubscriber(Types::ModuleIdentifier identifier, uint32_t& subscribeEventID) {
    std::unique_lock uniqueLock{sharedMutex};
    auto iter = std::find_if(std::begin(events), std::end(events), [&](auto& event) { return event.id == subscribeEventID; });
    if (iter != std::end(events)) {
        iter->subscribers.emplace_back(identifier);
    }
}

} // namespace Service