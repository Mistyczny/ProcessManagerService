#pragma once
#include "ServiceWatchdogConnectionState.hpp"
#include "WatchdogService.pb.h"
#include <string>

namespace Service {

class WatchdogResponseHandlerException : std::exception {
public:
    enum class ErrorCode { ResendConnectRequest, ResendPingRequest, ResendReconnectRequest, FailedToParse };

private:
    ErrorCode errorCode;

public:
    explicit WatchdogResponseHandlerException(ErrorCode errorCode);
    ~WatchdogResponseHandlerException() override = default;

    ErrorCode getErrorCode();
};

class WatchdogResponseHandler {
protected:
    uint32_t& sequenceCode;

public:
    explicit WatchdogResponseHandler(uint32_t& sequenceCode);
    virtual ~WatchdogResponseHandler() = default;

    /// @param Received message to handle as string
    virtual void handleResponse(std::string&) = 0;
};

class WatchdogConnectResponseHandler : public WatchdogResponseHandler {
protected:
    WatchdogConnectionState& watchdogConnectionState;
    WatchdogService::ConnectResponseData connectResponseData{};

    void handleConnectResponse();
    void handleReceivedConnectFailure();
    void handleResponseInvalidConnectionState();

public:
    explicit WatchdogConnectResponseHandler(uint32_t& sequenceCode, WatchdogConnectionState& watchdogConnectionState);
    ~WatchdogConnectResponseHandler() override = default;

    /// @param Received message to handle as string
    void handleResponse(std::string&) override;
};

class WatchdogPingResponseHandler : public WatchdogResponseHandler {
protected:
    WatchdogService::PingResponseData pingResponseData{};
    std::function<void()> pingTimerSet;

public:
    explicit WatchdogPingResponseHandler(uint32_t& sequenceCode, std::function<void()> pingTimerSet);
    ~WatchdogPingResponseHandler() override = default;

    /// @param Received message to handle as string
    void handleResponse(std::string&) override;
};

class WatchdogReconnectResponseHandler : public WatchdogResponseHandler {
protected:
    WatchdogConnectionState& watchdogConnectionState;
    WatchdogService::ReconnectResponseData reconnectResponseData{};

    void handleReconnectResponse();
    void handleReceivedReconnectFailure();
    void handleResponseInvalidReconnectionState();

public:
    explicit WatchdogReconnectResponseHandler(uint32_t& sequenceCode, WatchdogConnectionState& watchdogConnectionState);
    ~WatchdogReconnectResponseHandler() override = default;

    /// @param Received message to handle as string
    void handleResponse(std::string&) override;
};

} // namespace Service