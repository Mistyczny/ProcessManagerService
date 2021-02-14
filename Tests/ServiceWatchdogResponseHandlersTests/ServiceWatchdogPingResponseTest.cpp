#include "Communication.hpp"
#include "ServiceWatchdogConnectionState.hpp"
#include "ServiceWatchdogResponseHandlers.hpp"
#include "Types.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Testing service ping functionality", "") {
    uint32_t sequenceCode{};
    Service::WatchdogPingResponseHandler watchdogPingResponseHandler{sequenceCode};

    SECTION("Parsing invalid message") {
        std::string invalidMessage{"abcd"}; // Just random string
        REQUIRE_THROWS_AS(watchdogPingResponseHandler.handleResponse(invalidMessage), Service::WatchdogResponseHandlerException);
    }

    SECTION("Sequence code is valid") {
        sequenceCode = 1;
        auto pingMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
        WatchdogService::PingResponseData pingResponseData{};
        pingResponseData.set_sequencecode(1);
        pingResponseData.SerializeToString(&pingMessage->body);

        watchdogPingResponseHandler.handleResponse(pingMessage->body);
    }

    SECTION("Sequence code is invalid") {
        sequenceCode = 5;
        auto pingMessage = std::make_unique<Communication::Message<WatchdogService::Operation>>();
        WatchdogService::PingResponseData pingResponseData{};
        pingResponseData.set_sequencecode(1);
        pingResponseData.SerializeToString(&pingMessage->body);
        REQUIRE_THROWS_AS(watchdogPingResponseHandler.handleResponse(pingMessage->body), Service::WatchdogResponseHandlerException);
    }
}