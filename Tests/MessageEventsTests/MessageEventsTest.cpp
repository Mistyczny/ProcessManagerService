#include "Event.hpp"
#include "EventManager.hpp"
#include "Example.pb.h"
#include <algorithm>
#include <catch2/catch.hpp>
#include <iostream>

class SimpleUpdateMessageClass {
private:
public:
    bool validate(const google::protobuf::Any& any) { return any.Is<ExamplePrototypes::CoordinatesUpdate>(); }
    void run(const google::protobuf::Any& any) { std::cout << "DABI ABU" << std::endl; }
};

class SimpleGetMessageClass {
private:
public:
    bool validate(const google::protobuf::Any& any) { return any.Is<ExamplePrototypes::CoordinatesGet>(); }
    void run(const google::protobuf::Any& any) {
        ExamplePrototypes::CoordinatesGet coordinatesGet{};
        any.UnpackTo(&coordinatesGet);
        std::cout << coordinatesGet.identifier() << std::endl;
        std::cout << "ABU DABI" << std::endl;
    }
};

TEST_CASE("Testing message events functionality", "") {
    std::vector<std::unique_ptr<EventInterface>> events;

    ExamplePrototypes::CoordinatesUpdate updateMessage{};
    google::protobuf::Any anyUpdateMessage{};
    anyUpdateMessage.PackFrom(updateMessage);

    auto simpleUpdateEventClass = std::make_shared<SimpleUpdateMessageClass>();
    std::unique_ptr<EventInterface> updateMessageEvent =
        std::make_unique<MessageEvent<std::shared_ptr<SimpleUpdateMessageClass>>>(simpleUpdateEventClass);
    std::cout << updateMessageEvent->validateMessage(anyUpdateMessage) << std::endl;
    updateMessageEvent->handleReceivedMessage(anyUpdateMessage);

    ExamplePrototypes::CoordinatesGet getMessage{};
    getMessage.set_identifier(17);
    google::protobuf::Any anyGetMessage{};
    anyGetMessage.PackFrom(getMessage);

    auto simpleEventClass = std::make_shared<SimpleGetMessageClass>();
    std::unique_ptr<EventInterface> getMessageEvent =
        std::make_unique<MessageEvent<std::shared_ptr<SimpleGetMessageClass>>>(simpleEventClass);
    std::cout << getMessageEvent->validateMessage(anyGetMessage) << std::endl;
    getMessageEvent->handleReceivedMessage(anyGetMessage);

    events.push_back(std::move(updateMessageEvent));
    events.push_back(std::move(getMessageEvent));

    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (event->validateMessage(anyGetMessage)) {
            event->handleReceivedMessage(anyGetMessage);
        } else {
            std::cout << "VALIDATION FAILURE" << std::endl;
        }
    });
}