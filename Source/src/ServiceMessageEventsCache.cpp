#include "ServiceMessageEventsCache.hpp"
#include <optional>

namespace Service {

uint32_t MessageEventsCache::getNewEventID() const {
    uint32_t availableEventID{};
    for (auto& iter : events) {
        if (iter.first == availableEventID) {
            availableEventID++;
        } else {
            Log::trace("Found empty message event id = " + std::to_string(availableEventID));
            break;
        }
    }
    return availableEventID;
}

uint32_t MessageEventsCache::addMessageEvent(std::unique_ptr<EventInterface> newEvent) {
    uint32_t messageEventID{};
    std::unique_lock uniqueLock{sharedMutex};
    if (newEvent) {
        messageEventID = this->getNewEventID();
        events.emplace_back(messageEventID, std::move(newEvent));
    } else {
        throw std::runtime_error("Trying to add null message event");
    }
    return messageEventID;
}

std::vector<google::protobuf::Any*> MessageEventsCache::triggerMessageHandlers(const Service::Sender sender,
                                                                              const google::protobuf::Any& AnyMessage) {
    std::shared_lock sharedLock{sharedMutex};
    std::vector<google::protobuf::Any*> responses{};
    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (event.second->validateMessage(AnyMessage)) {
            auto response = event.second->handleReceivedMessage(sender, AnyMessage);
            if (response.has_value()) {
                responses.emplace_back(*response);
            }
        }
    });
    return responses;
}

size_t MessageEventsCache::size() const {
    std::shared_lock sharedLock{sharedMutex};
    return this->events.size();
}

} // namespace Service