#include "GPS.pb.h"
#include "GpsCoordinates.hpp"
#include "Service.hpp"
#include "ServiceGlobals.hpp"
#include "ServiceTask.hpp"

namespace Service {

class SimpleUpdateMessageClass {
private:
    std::shared_ptr<GpsCoordinates> gpsCoordinates;

public:
    explicit SimpleUpdateMessageClass(std::shared_ptr<GpsCoordinates> gpsCoordinates) : gpsCoordinates{gpsCoordinates} {}

    bool validate(const google::protobuf::Any& any) { return any.Is<GPS::CoordinatesUpdateRequest>(); }

    std::optional<google::protobuf::Any*> run(const Service::Sender sender, const google::protobuf::Any& any) {
        std::optional<google::protobuf::Any> response{std::nullopt};
        GPS::CoordinatesUpdateRequest coordinatesUpdateRequest{};
        any.UnpackTo(&coordinatesUpdateRequest);
        std::cout << "Updating X: " << std::to_string(coordinatesUpdateRequest.x()) << std::endl;
        std::cout << "Updating Y: " << std::to_string(coordinatesUpdateRequest.y()) << std::endl;
        gpsCoordinates->setCoordinates(sender.senderIdentifier, std::make_pair(coordinatesUpdateRequest.x(), coordinatesUpdateRequest.y()));

        GPS::CoordinatesUpdateResponse coordinatesUpdateResponse{};
        coordinatesUpdateResponse.set_identifier(sender.senderIdentifier);
        coordinatesUpdateResponse.set_responsecode(GPS::CoordinatesUpdateResponse::SUCCESS);

        return {};
    }
};

class SimpleGetMessageClass {
private:
    std::shared_ptr<GpsCoordinates> gpsCoordinates;

public:
    explicit SimpleGetMessageClass(std::shared_ptr<GpsCoordinates> gpsCoordinates) : gpsCoordinates{gpsCoordinates} {}
    bool validate(const google::protobuf::Any& any) { return any.Is<GPS::CoordinatesGetRequest>(); }
    std::optional<google::protobuf::Any*> run(const Service::Sender, const google::protobuf::Any& any) {
        std::optional<google::protobuf::Any> response{std::nullopt};
        GPS::CoordinatesGetRequest coordinatesGetRequest{};
        any.UnpackTo(&coordinatesGetRequest);
        return {};
    }
};

class SubscribeUpdateEvent {
private:
    std::shared_ptr<GpsCoordinates> gpsCoordinates;

public:
    explicit SubscribeUpdateEvent(std::shared_ptr<GpsCoordinates> gpsCoordinates) : gpsCoordinates{gpsCoordinates} {}
    bool validate(const google::protobuf::Any& any) { return any.Is<GPS::SubscribeIdentifierRequest>(); }
    std::optional<google::protobuf::Any*> run(const Service::Sender, const google::protobuf::Any& any) {
        std::optional<google::protobuf::Any> response{std::nullopt};
        GPS::CoordinatesGetRequest coordinatesGetRequest{};
        any.UnpackTo(&coordinatesGetRequest);
        return {};
    }
};

class SimpleSubscribeUpdateMessage {
private:
    std::shared_ptr<GpsCoordinates> gpsCoordinates;

public:
    explicit SimpleSubscribeUpdateMessage(std::shared_ptr<GpsCoordinates> gpsCoordinates) : gpsCoordinates{gpsCoordinates} {}
    std::optional<google::protobuf::Any*> run(const google::protobuf::Any& any) {
        std::cout << "HERE" << std::endl;
        std::optional<google::protobuf::Any> response{std::nullopt};
        GPS::CoordinatesUpdateRequest coordinatesUpdateRequest{};
        any.UnpackTo(&coordinatesUpdateRequest);

        auto* anyMessage = new google::protobuf::Any();
        GPS::SubscribeIdentifierRequest subscribeIdentifierRequest{};
        subscribeIdentifierRequest.set_identifier(coordinatesUpdateRequest.identifier());
        subscribeIdentifierRequest.set_x(coordinatesUpdateRequest.x());
        subscribeIdentifierRequest.set_y(coordinatesUpdateRequest.y());
        anyMessage->PackFrom(subscribeIdentifierRequest);
        return anyMessage;
    }
};

Task::Task() { Service::Globals::serviceIdentifier = 872415233; }

Task::~Task() = default;

int Task::run() {
    auto gpsCoordinates = std::make_shared<GpsCoordinates>();

    // Registering message handlers
    auto simpleUpdateEventClass = std::make_shared<SimpleUpdateMessageClass>(gpsCoordinates);
    auto updateMessageEvent = std::make_unique<MessageEvent<std::shared_ptr<SimpleUpdateMessageClass>>>(simpleUpdateEventClass);

    auto simpleGetEventClass = std::make_shared<SimpleGetMessageClass>(gpsCoordinates);
    auto getMessageEvent = std::make_unique<MessageEvent<std::shared_ptr<SimpleGetMessageClass>>>(simpleGetEventClass);

    // Register subsciption event
    auto simpleSubscribeUpdateMessage = std::make_shared<SimpleSubscribeUpdateMessage>(gpsCoordinates);
    std::unique_ptr<SubscribeEventInterface> simpleSubscribeUpdateMessageEvent =
        std::make_unique<SubscribeEvent<std::shared_ptr<SimpleSubscribeUpdateMessage>>>(simpleSubscribeUpdateMessage);

    EventManager::registerNewEventHandler(1, std::move(updateMessageEvent));
    EventManager::registerNewEventHandler(1, std::move(getMessageEvent));
    GPS::CoordinatesUpdateRequest updateRequest{};
    EventManager::registerNewSubscribtionHandler(updateRequest.GetTypeName(), std::move(simpleSubscribeUpdateMessageEvent));

    return Service::ReturnCodes::RUN_FOREVER;
}

} // namespace Service