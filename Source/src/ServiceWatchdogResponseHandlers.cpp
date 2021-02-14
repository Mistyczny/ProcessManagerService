#include "ServiceWatchdogResponseHandlers.hpp"
#include "Logging.hpp"

namespace Service {

WatchdogResponseHandlerException::WatchdogResponseHandlerException(ErrorCode errorCode) { this->errorCode = errorCode; }

WatchdogResponseHandlerException::ErrorCode WatchdogResponseHandlerException::getErrorCode() { return this->errorCode; }

WatchdogResponseHandler::WatchdogResponseHandler(uint32_t& sequenceCode) : sequenceCode{sequenceCode} {}

WatchdogConnectResponseHandler::WatchdogConnectResponseHandler(uint32_t& sequenceCode, WatchdogConnectionState& watchdogConnectionState)
    : WatchdogResponseHandler{sequenceCode}, watchdogConnectionState{watchdogConnectionState} {}

void WatchdogConnectResponseHandler::handleResponse(std::string& receivedResponse) {
    if (!this->connectResponseData.ParseFromString(receivedResponse)) {
        Log::error("Failed to parse received watchdog connect response");
        throw WatchdogResponseHandlerException{WatchdogResponseHandlerException::ErrorCode::FailedToParse};
    } else {
        switch (this->watchdogConnectionState.getConnectionState()) {
        case WatchdogConnectionState::ConnectionState::NotConnected:
            this->handleConnectResponse();
            break;
        case WatchdogConnectionState::ConnectionState::Connected:
            Log::error("Received connect response when service is in connected state");
            break;
        case WatchdogConnectionState::ConnectionState::Shutdown:
            Log::error("Received connect response when service is shutting down");
            break;
        default:
            Log::critical("Connection state has invalid value: " +
                          std::to_string(static_cast<int>(this->watchdogConnectionState.getConnectionState())));
        }
    }
}

void WatchdogConnectResponseHandler::handleConnectResponse() {
    WatchdogConnectionState::ConnectionState newWatchdogConnectionState{};
    if (!connectResponseData.has_sequencecode()) {
        Log::critical("Received connect response that doesn't have sequence code");
    } else if (connectResponseData.responsecode() != WatchdogService::ResponseCode::Success) {
        this->handleReceivedConnectFailure();
    } else {
        Log::info("Successfully connected to watchdog");
        this->sequenceCode = connectResponseData.sequencecode();
        this->watchdogConnectionState.setConnectionStateAndNotify(WatchdogConnectionState::ConnectionState::Connected);
    }
}

void WatchdogConnectResponseHandler::handleReceivedConnectFailure() {
    switch (this->connectResponseData.responsecode()) {
    case WatchdogService::ResponseCode::InvalidConnectionState:
        this->handleResponseInvalidConnectionState();
        break;
    case WatchdogService::ResponseCode::NotServiceIdentifier:
        break;
    case WatchdogService::ResponseCode::ServiceNotExists:
        Log::critical("Service doesnt exists");
        break;
    case WatchdogService::ResponseCode::Unknown:
    default:
        Log::error("Unexpected behaviour happened, received unknown response code");
    }
}

void WatchdogConnectResponseHandler::handleResponseInvalidConnectionState() {
    if (this->connectResponseData.has_connectionstate()) {
        switch (this->connectResponseData.connectionstate()) {
        case WatchdogService::ConnectionState::Registered:
            Log::error("Received invalid connection state, but state was registered, try to resend connect request");
            throw WatchdogResponseHandlerException(WatchdogResponseHandlerException::ErrorCode::ResendConnectRequest);
        case WatchdogService::ConnectionState::Connected:
            Log::info("Service is already connected");
            break;
        case WatchdogService::ConnectionState::Disconnected:
            Log::error("Received invalid connection state, but state was disconnected, try to resend connect request");
            throw WatchdogResponseHandlerException(WatchdogResponseHandlerException::ErrorCode::ResendConnectRequest);
        default:
            Log::error("Service is already connected");
            break;
        }
    } else {
        Log::error("Received invalid connection state, but current connection state was not included");
    }
}

void WatchdogPingResponseHandler::handleResponse(std::string& receivedResponse) {
    if (!this->pingResponseData.ParseFromString(receivedResponse)) {
        Log::error("Failed to parse received watchdog connect response");
        throw WatchdogResponseHandlerException{WatchdogResponseHandlerException::ErrorCode::FailedToParse};
    } else {
        if (this->sequenceCode == pingResponseData.sequencecode()) {
            // all ok, restart timer
        } else {
            Log::info("Received ping with invalid sequence code");
            throw WatchdogResponseHandlerException(WatchdogResponseHandlerException::ErrorCode::ResendPingRequest);
        }
    }
}

WatchdogPingResponseHandler::WatchdogPingResponseHandler(uint32_t& sequenceCode) : WatchdogResponseHandler(sequenceCode) {}

WatchdogReconnectResponseHandler::WatchdogReconnectResponseHandler(uint32_t& sequenceCode, WatchdogConnectionState& watchdogConnectionState)
    : WatchdogResponseHandler{sequenceCode}, watchdogConnectionState{watchdogConnectionState} {}

void WatchdogReconnectResponseHandler::handleResponse(std::string& receivedResponse) {
    if (!this->reconnectResponseData.ParseFromString(receivedResponse)) {
        Log::error("Failed to parse received watchdog connect response");
        throw WatchdogResponseHandlerException{WatchdogResponseHandlerException::ErrorCode::FailedToParse};
    } else {
        switch (this->watchdogConnectionState.getConnectionState()) {
        case WatchdogConnectionState::ConnectionState::NotConnected:
            this->handleReconnectResponse();
            break;
        case WatchdogConnectionState::ConnectionState::Connected:
            Log::error("Received reconnect response when service is in connected state");
            break;
        case WatchdogConnectionState::ConnectionState::Shutdown:
            Log::error("Received reconnect response when service is shutting down");
            break;
        default:
            Log::critical("Connection state has invalid value: " +
                          std::to_string(static_cast<int>(this->watchdogConnectionState.getConnectionState())));
        }
    }
}

void WatchdogReconnectResponseHandler::handleReconnectResponse() {
    if (reconnectResponseData.responsecode() != WatchdogService::ResponseCode::Success) {
        this->handleReceivedReconnectFailure();
    } else if (!reconnectResponseData.has_sequencecode()) {
        Log::critical("Received connect response that doesn't have sequence code");
    } else {
        Log::info("Successfully connected to watchdog");
        this->sequenceCode = reconnectResponseData.sequencecode();
        this->watchdogConnectionState.setConnectionStateAndNotify(WatchdogConnectionState::ConnectionState::Connected);
    }
}

void WatchdogReconnectResponseHandler::handleReceivedReconnectFailure() {
    switch (this->reconnectResponseData.responsecode()) {
    case WatchdogService::ResponseCode::InvalidConnectionState:
        this->handleResponseInvalidReconnectionState();
        break;
    case WatchdogService::ResponseCode::NotServiceIdentifier:
        break;
    case WatchdogService::ResponseCode::ServiceNotExists:
        Log::critical("");
        break;
    case WatchdogService::ResponseCode::Unknown:
    default:
        Log::error("Unexpected behaviour happened, received unknown response code");
    }
}

void WatchdogReconnectResponseHandler::handleResponseInvalidReconnectionState() {
    if (this->reconnectResponseData.has_connectionstate()) {
        switch (this->reconnectResponseData.connectionstate()) {
        case WatchdogService::ConnectionState::Registered:
            Log::error("Received invalid connection state, but state was registered, try to resend connect request");
            throw WatchdogResponseHandlerException(WatchdogResponseHandlerException::ErrorCode::ResendReconnectRequest);
        case WatchdogService::ConnectionState::Connected:
            Log::info("Service is already connected");
            break;
        case WatchdogService::ConnectionState::Disconnected:
            Log::error("Received invalid connection state, but state was disconnected, try to resend connect request");
            throw WatchdogResponseHandlerException(WatchdogResponseHandlerException::ErrorCode::ResendReconnectRequest);
        default:
            Log::error("Service is already connected");
            break;
        }
    } else {
        Log::error("Received invalid connection state, but current connection state was not included");
    }
}

} // namespace Service