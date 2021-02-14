#include "Communication.hpp"
#include "ServiceWatchdogConnectionState.hpp"
#include "ServiceWatchdogResponseHandlers.hpp"
#include "Types.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Testing service-watchdog connect functionality", "") {
    uint32_t sequenceCode{};
    Service::WatchdogConnectionState watchdogConnectionState{};
    Service::WatchdogReconnectResponseHandler watchdogReconnectResponseHandler{sequenceCode, watchdogConnectionState};

    SECTION("Parsing invalid message") {
        std::string invalidMessage{"abcd"}; // Just random string
        REQUIRE_THROWS_AS(watchdogReconnectResponseHandler.handleResponse(invalidMessage), Service::WatchdogResponseHandlerException);
    }

    SECTION("Received success") {
        sequenceCode = 5;

        SECTION("Has sequence code") {
            auto connectMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
            WatchdogService::ConnectResponseData connectResponseData{};
            connectResponseData.set_responsecode(WatchdogService::Success);
            connectResponseData.set_sequencecode(3);
            connectResponseData.SerializeToString(&connectMessage->body);
            watchdogReconnectResponseHandler.handleResponse(connectMessage->body);
            REQUIRE(watchdogConnectionState.getConnectionState() == Service::WatchdogConnectionState::ConnectionState::Connected);
            REQUIRE(sequenceCode == 3);
        }

        SECTION("Doesnt have sequence code") {
            auto connectMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
            WatchdogService::ConnectResponseData connectResponseData{};
            connectResponseData.set_responsecode(WatchdogService::Success);
            connectResponseData.SerializeToString(&connectMessage->body);
            watchdogReconnectResponseHandler.handleResponse(connectMessage->body);
            REQUIRE(connectResponseData.has_sequencecode() == false);
            REQUIRE(watchdogConnectionState.getConnectionState() == Service::WatchdogConnectionState::ConnectionState::NotConnected);
        }
    }

    SECTION("Received InvalidConnectionState") {
        auto connectMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
        WatchdogService::ConnectResponseData connectResponseData{};
        connectResponseData.set_responsecode(WatchdogService::InvalidConnectionState);
        connectResponseData.set_sequencecode(5);
        connectResponseData.set_connectionstate(WatchdogService::ConnectionState::Registered);
        connectResponseData.SerializeToString(&connectMessage->body);
        REQUIRE_THROWS_AS(watchdogReconnectResponseHandler.handleResponse(connectMessage->body), Service::WatchdogResponseHandlerException);
        REQUIRE(watchdogConnectionState.getConnectionState() == Service::WatchdogConnectionState::ConnectionState::NotConnected);
    }

    SECTION("Received NotModuleIdentifier") {
        auto connectMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
        WatchdogService::ConnectResponseData connectResponseData{};
        connectResponseData.set_responsecode(WatchdogService::NotServiceIdentifier);
        connectResponseData.SerializeToString(&connectMessage->body);
        watchdogReconnectResponseHandler.handleResponse(connectMessage->body);
        REQUIRE(watchdogConnectionState.getConnectionState() == Service::WatchdogConnectionState::ConnectionState::NotConnected);
        REQUIRE(connectResponseData.has_sequencecode() == false);
    }

    SECTION("Received ServiceNotExists") {
        auto connectMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
        WatchdogService::ConnectResponseData connectResponseData{};
        connectResponseData.set_responsecode(WatchdogService::ServiceNotExists);
        connectResponseData.SerializeToString(&connectMessage->body);
        watchdogReconnectResponseHandler.handleResponse(connectMessage->body);
        REQUIRE(watchdogConnectionState.getConnectionState() == Service::WatchdogConnectionState::ConnectionState::NotConnected);
        REQUIRE(connectResponseData.has_sequencecode() == false);
    }
}