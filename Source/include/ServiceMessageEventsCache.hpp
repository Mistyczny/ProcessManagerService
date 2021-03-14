#pragma once
#include "Event.hpp"
#include "shared_mutex"
#include <google/protobuf/any.pb.h>

namespace Service {

class MessageEventsCache {
private:
    mutable std::shared_mutex sharedMutex;
    std::vector<std::pair<uint32_t, std::unique_ptr<EventInterface>>> events;

    uint32_t getNewEventID() const;

public:
    MessageEventsCache() = default;
    ~MessageEventsCache() = default;

    uint32_t addMessageEvent(std::unique_ptr<EventInterface> newEvent);
    std::vector<google::protobuf::Any> triggerMessageHandlers(const Service::Sender sender, const google::protobuf::Any& any);

    size_t size() const;
};

} // namespace Service