#pragma once
#include "Logging.hpp"
#include "ServiceCommon.hpp"
#include "Types.hpp"
#include <chrono>
#include <functional>
#include <google/protobuf/any.pb.h>
#include <memory>
#include <mutex>
#include <string>

class EventInterface {
protected:
public:
    EventInterface() = default;
    virtual ~EventInterface() = default;

    virtual bool validateMessage(const google::protobuf::Any& any) = 0;
    virtual std::optional<google::protobuf::Any*> handleReceivedMessage(const Service::Sender sender, const google::protobuf::Any& any) = 0;
};

template <class T, std::enable_if_t<!std::is_pointer_v<T>, bool> = true> class MessageEvent : public EventInterface {
protected:
    T callback{};

public:
    MessageEvent() = default;
    explicit MessageEvent(T callback) { this->callback = callback; }
    ~MessageEvent() override = default;

    bool validateMessage(const google::protobuf::Any& any) override {
        bool messageValidated{false};
        if (this->callback) {
            messageValidated = this->callback->validate(any);
        }
        return messageValidated;
    }

    std::optional<google::protobuf::Any*> handleReceivedMessage(const Service::Sender sender, const google::protobuf::Any& any) override {
        return this->callback->run(sender, any);
    }
};

class SubscribeEventInterface {
protected:
public:
    SubscribeEventInterface() = default;
    virtual ~SubscribeEventInterface() = default;

    virtual std::optional<google::protobuf::Any*> subscriptionHandle(google::protobuf::Any&) = 0;
};

template <class T, std::enable_if_t<!std::is_pointer_v<T>, bool> = true> class SubscribeEvent : public SubscribeEventInterface {
protected:
    T callback{};

public:
    SubscribeEvent() = default;
    explicit SubscribeEvent(T callback) { this->callback = callback; }
    ~SubscribeEvent() override = default;

    std::optional<google::protobuf::Any*> subscriptionHandle(google::protobuf::Any& any) override { return this->callback->run(any); }
};

struct Subscription {
    uint32_t id;
    std::string messageType;
    std::vector<Types::ModuleIdentifier> subscribers;
    std::unique_ptr<SubscribeEventInterface> event;
};