#pragma once
#include "Connection.hpp"
#include "ServiceWatchdogResponseHandlers.hpp"
#include "WatchdogService.pb.h"
#include <memory>

namespace Service {

class WatchdogConnection : public Connection::TcpConnection<WatchdogService::Operation> {
protected:
    WatchdogConnectionState& watchdogConnectionState;
    uint32_t sequenceCode;

    void handleReceivedMessage(std::unique_ptr<Communication::Message<WatchdogService::Operation>> receivedMessage) override;
    void onTimerExpiration() override;

    std::unique_ptr<WatchdogResponseHandler> getWatchdogResponseHandler(const WatchdogService::Operation& operationCode);
    void handleResponse(std::unique_ptr<WatchdogResponseHandler>, std::string&);
    void processCaughtResponseException(WatchdogResponseHandlerException& exception);

    void sendConnectRequest();
    void sendPingRequest();
    void sendReconnectRequest();
    void sendShutdownRequest();
public:
    explicit WatchdogConnection(boost::asio::io_context& ioContext, WatchdogConnectionState& watchdogConnectionState);
    ~WatchdogConnection() override = default;
};

} // namespace Service