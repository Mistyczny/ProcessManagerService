/**
 * @file ServiceSubscriptionEventsCache.hpp
 * @brief Service subscriptions events cache
 * @author Kacper Waśniowski
 * @copyright Kacper Waśniowski, All Rights Reserved
 * @date 2021
 */
#include "ServiceSubscriptionEventsCache.hpp"

#include <utility>

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

uint32_t SubscriptionEventsCache::addSubscriptionEvent(std::string subscribeType, std::unique_ptr<SubscribeEventInterface> newEvent) {
    uint32_t subscriptionEventID{};
    std::unique_lock uniqueLock{sharedMutex};
    if (newEvent) {
        std::cout << " NEW SUBSCRIPTION EVENT ADDED" << std::endl;
        subscriptionEventID = this->getNewEventID();
        Subscription newSubscription{};
        newSubscription.id = subscriptionEventID;
        newSubscription.event = std::move(newEvent);
        newSubscription.messageType = std::move(subscribeType);
        events.emplace_back(std::move(newSubscription));
    } else {
        throw std::runtime_error("Trying to add null message event");
    }
    return subscriptionEventID;
}

std::vector<std::pair<Types::ModuleIdentifier, google::protobuf::Any*>>
SubscriptionEventsCache::triggerMessageHandlers(google::protobuf::Any& anyMessage) {
    std::shared_lock sharedLock{sharedMutex};
    std::vector<std::pair<Types::ModuleIdentifier, google::protobuf::Any*>> serviceRequests{};
    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (anyMessage.type_url().ends_with(event.messageType)) {
            std::cout << "TYPE SUBSCRIBE RECIVED: " << event.messageType << std::endl;
            std::for_each(std::begin(event.subscribers), std::end(event.subscribers), [&](auto& subscriber) {
                auto newMessage = event.event->subscriptionHandle(anyMessage);
                if (newMessage.has_value()) {
                    std::cout << "PUTTING RESPONSE" << std::endl;
                    serviceRequests.push_back(std::make_pair(subscriber, std::move(*newMessage)));
                    std::cout << "POST PUTTING RESPONSE" << std::endl;
                } else {
                    std::cout << "NO VALUE" << std::endl;
                }
            });
        } else {
            std::cout << "TYPE DOESNT MATCH: " << event.messageType << " " << anyMessage.type_url() << std::endl;
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
    std::cout << " NEW SUBSCRIBER EVENT ADDED" << std::endl;
    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (event.messageType == typeUrl) {
            std::cout << "TYPE MATCH" << std::endl;
            event.subscribers.emplace_back(moduleIdentifier);
        } else {
            std::cout << "TYPE DOESNT MATCH" << std::endl;
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