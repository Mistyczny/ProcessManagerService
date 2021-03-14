#pragma once
#include "Logging.hpp"
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
    virtual void handleReceivedMessage(const google::protobuf::Any& any) = 0;
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

    void handleReceivedMessage(const google::protobuf::Any& any) override { this->callback->run(any); }
};