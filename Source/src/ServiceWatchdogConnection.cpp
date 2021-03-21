#include "ServiceWatchdogConnection.hpp"
#include "ServiceGlobals.hpp"
#include <functional>

namespace Service {

WatchdogConnection::WatchdogConnection(boost::asio::io_context& ioContext, WatchdogConnectionState& watchdogConnectionState)
    : Connection::TcpConnection<WatchdogService::Operation>(ioContext), watchdogConnectionState{watchdogConnectionState} {}

void WatchdogConnection::handleReceivedMessage(std::unique_ptr<Communication::Message<WatchdogService::Operation>> receivedMessage) {
    if (!receivedMessage) {
        Log::error("WatchdogConnection::handleReceivedMessage received message is null");
    } else {
        auto& [header, body] = *receivedMessage;
        auto watchdogResponseHandler = this->getWatchdogResponseHandler(header.operationCode);
        if (watchdogResponseHandler) {
            watchdogResponseHandler->handleResponse(body);
        } else {
            Log::error("WatchdogConnection::handleReceivedMessage Failed to get watchdog response handler");
        }
    }
}

void WatchdogConnection::onTimerExpiration() { this->sendPingRequest(); }

std::unique_ptr<WatchdogResponseHandler> WatchdogConnection::getWatchdogResponseHandler(const WatchdogService::Operation& operationCode) {
    std::unique_ptr<WatchdogResponseHandler> watchdogResponseHandler{nullptr};
    auto startPingTimer = std::bind([&]() { this->setTimerExpiration(3000); });
    std::cout << "RECEIVED OP CODE: " << operationCode << std::endl;
    switch (operationCode) {
    case WatchdogService::Operation::ConnectResponse:
        watchdogResponseHandler = std::make_unique<WatchdogConnectResponseHandler>(this->sequenceCode, this->watchdogConnectionState);
        break;
    case WatchdogService::Operation::PingResponse:
        watchdogResponseHandler = std::make_unique<WatchdogPingResponseHandler>(this->sequenceCode, std::move(startPingTimer));
        break;
    case WatchdogService::Operation::ReconnectResponse:
        watchdogResponseHandler = std::make_unique<WatchdogReconnectResponseHandler>(this->sequenceCode, this->watchdogConnectionState);
        break;
    default:
        break;
    }
    return watchdogResponseHandler;
}

void WatchdogConnection::handleResponse(std::unique_ptr<WatchdogResponseHandler> watchdogResponseHandler, std::string& receivedResponse) {
    if (watchdogResponseHandler) {
        try {
            watchdogResponseHandler->handleResponse(receivedResponse);
        } catch (WatchdogResponseHandlerException& exception) {
            this->processCaughtResponseException(exception);
        } catch (std::exception& ex) {
            Log::critical("Caught exception");
        } catch (...) {
            Log::critical("Caught unexpected exception");
        }
    } else {
        Log::error("WatchdogConnection::handleResponse - watchdog response handler is null");
    }
}

void WatchdogConnection::processCaughtResponseException(WatchdogResponseHandlerException& watchdogResponseHandlerException) {
    switch (watchdogResponseHandlerException.getErrorCode()) {
    case WatchdogResponseHandlerException::ErrorCode::FailedToParse:
        break;
    case WatchdogResponseHandlerException::ErrorCode::ResendConnectRequest:
        this->sendConnectRequest();
        break;
    case WatchdogResponseHandlerException::ErrorCode::ResendPingRequest:
        this->sendPingRequest();
        break;
    case WatchdogResponseHandlerException::ErrorCode::ResendReconnectRequest:
        this->sendReconnectRequest();
        break;
    default:
        Log::error("WatchdogConnection::processCaughtResponseException - Invalid error code");
    }
}

void WatchdogConnection::sendConnectRequest() {
    Log::trace("WatchdogConnection::sendConnectRequest");
    WatchdogService::ConnectRequestData connectRequestData{};
    connectRequestData.set_identifier(Globals::serviceIdentifier);

    Communication::Message<WatchdogService::Operation> connectMessage{};
    connectRequestData.SerializeToString(&connectMessage.body);
    connectMessage.header.operationCode = WatchdogService::Operation::ConnectRequest;
    connectMessage.header.size = connectMessage.body.size();

    this->sendMessage(connectMessage);
}

void WatchdogConnection::sendPingRequest() {
    Log::trace("WatchdogConnection::sendPingRequest");
    WatchdogService::PingRequestData pingRequestData{};
    pingRequestData.set_sequencecode(this->sequenceCode);

    Communication::Message<WatchdogService::Operation> pingMessage{};
    pingRequestData.SerializeToString(&pingMessage.body);
    pingMessage.header.operationCode = WatchdogService::Operation::PingRequest;
    pingMessage.header.size = pingMessage.body.size();

    this->sendMessage(pingMessage);
}

void WatchdogConnection::sendReconnectRequest() {
    Log::trace("WatchdogConnection::sendReconnectRequest");
    WatchdogService::ReconnectRequestData reconnectRequestData{};
    reconnectRequestData.set_identifier(Globals::serviceIdentifier);

    Communication::Message<WatchdogService::Operation> reconnectMessage{};
    reconnectRequestData.SerializeToString(&reconnectMessage.body);
    reconnectMessage.header.operationCode = WatchdogService::Operation::ReconnectRequest;
    reconnectMessage.header.size = reconnectMessage.body.size();

    this->sendMessage(reconnectMessage);
}

void WatchdogConnection::sendShutdownRequest() {
    Log::trace("WatchdogConnection::sendShutdownRequest");
    WatchdogService::ShutdownRequestData shutdownRequestData{};
    shutdownRequestData.set_identifier(Globals::serviceIdentifier);

    Communication::Message<WatchdogService::Operation> shutdownMessage{};
    shutdownRequestData.SerializeToString(&shutdownMessage.body);
    shutdownMessage.header.operationCode = WatchdogService::Operation::ShutdownRequest;
    shutdownMessage.header.size = shutdownMessage.body.size();

    this->sendMessage(shutdownMessage);
}

} // namespace Service