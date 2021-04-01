#include "Event.hpp"
#include "EventManager.hpp"
#include "Example.pb.h"
#include "ServiceCommon.hpp"
#include <algorithm>
#include <catch2/catch.hpp>
#include <iostream>

class SimpleUpdateMessageClass {
private:
public:
    bool validate(const google::protobuf::Any& any) { return any.Is<ExamplePrototypes::CoordinatesUpdate>(); }
    std::optional<google::protobuf::Any*> run(const Service::Sender, const google::protobuf::Any& any) { return {}; }
};

class SimpleGetMessageClass {
private:
public:
    bool validate(const google::protobuf::Any& any) { return any.Is<ExamplePrototypes::CoordinatesGet>(); }
    std::optional<google::protobuf::Any*> run(const Service::Sender, const google::protobuf::Any& any) {
        ExamplePrototypes::CoordinatesGet coordinatesGet{};
        any.UnpackTo(&coordinatesGet);
        std::cout << coordinatesGet.identifier() << std::endl;
        return {};
    }
};

TEST_CASE("Testing message events functionality", "") {
    std::vector<std::unique_ptr<EventInterface>> events;
    const Service::Sender sender{};

    ExamplePrototypes::CoordinatesUpdate updateMessage{};
    google::protobuf::Any anyUpdateMessage{};
    anyUpdateMessage.PackFrom(updateMessage);

    auto simpleUpdateEventClass = std::make_shared<SimpleUpdateMessageClass>();
    std::unique_ptr<EventInterface> updateMessageEvent =
        std::make_unique<MessageEvent<std::shared_ptr<SimpleUpdateMessageClass>>>(simpleUpdateEventClass);
    std::cout << updateMessageEvent->validateMessage(anyUpdateMessage) << std::endl;
    updateMessageEvent->handleReceivedMessage(sender, anyUpdateMessage);

    ExamplePrototypes::CoordinatesGet getMessage{};
    getMessage.set_identifier(17);
    google::protobuf::Any anyGetMessage{};
    anyGetMessage.PackFrom(getMessage);

    auto simpleEventClass = std::make_shared<SimpleGetMessageClass>();
    std::unique_ptr<EventInterface> getMessageEvent =
        std::make_unique<MessageEvent<std::shared_ptr<SimpleGetMessageClass>>>(simpleEventClass);
    std::cout << getMessageEvent->validateMessage(anyGetMessage) << std::endl;
    getMessageEvent->handleReceivedMessage(sender, anyGetMessage);

    events.push_back(std::move(updateMessageEvent));
    events.push_back(std::move(getMessageEvent));

    std::for_each(std::begin(events), std::end(events), [&](auto& event) {
        if (event->validateMessage(anyGetMessage)) {
            event->handleReceivedMessage(sender, anyGetMessage);
        } else {
            std::cout << "VALIDATION FAILURE" << std::endl;
        }
    });

    google::protobuf::Any anyMsg{};
    anyMsg.PackFrom(getMessage);
    anyMsg.type_url();
    std::cout << "CHECK IF IS: " << anyGetMessage.type_url() << std::endl;
}