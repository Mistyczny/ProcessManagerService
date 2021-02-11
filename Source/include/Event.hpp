#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <string>

class EventInterface {
protected:
public:
    EventInterface() = default;
    virtual ~EventInterface() = default;

    virtual void handleReceivedMessage(std::string message) = 0;
};

template <typename T, std::enable_if_t<!std::is_pointer_v<T>, bool> = true> class MessageReceiveEvent : public EventInterface {
protected:
    T callbackData{};
    std::function<void(T, std::string)> callbackFunction{};

public:
    MessageReceiveEvent() = default;
    MessageReceiveEvent(std::function<void(T, std::string)> callbackFunction, T callbackData) {
        this->callbackData = callbackData;
        this->callbackFunction = callbackFunction;
    }
    ~MessageReceiveEvent() override = default;

    void run() { this->callbackFunction(callbackData); }

    void handleReceivedMessage(std::string message) override { this->callbackFunction(callbackData, message); }
};