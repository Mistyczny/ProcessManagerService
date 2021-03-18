/**
 * @file ServiceSubscriptionEventsCache.hpp
 * @brief Service subscriptions events cache
 * @author Kacper Waśniowski
 * @copyright Kacper Waśniowski, All Rights Reserved
 * @date 2021
 */
#pragma once
#include "Event.hpp"
#include "shared_mutex"
#include <google/protobuf/any.pb.h>

namespace Service {

class SubscriptionEventsCache {
private:
    mutable std::shared_mutex sharedMutex;
    std::vector<Subscription> events;

    uint32_t getNewEventID() const;

public:
    SubscriptionEventsCache() = default;
    virtual ~SubscriptionEventsCache() = default;

    uint32_t addSubscriptionEvent(std::unique_ptr<SubscribeEventInterface> newEvent);
    void addSubscriber(Types::ModuleIdentifier moduleIdentifier, std::string&);
    std::vector<std::pair<Types::ModuleIdentifier, google::protobuf::Any>> triggerMessageHandlers(google::protobuf::Any&);

    size_t size() const;
};

} // namespace Service