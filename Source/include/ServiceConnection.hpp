#pragma once
#include "Connection.hpp"
#include "WatchdogService.pb.h"

namespace Service {

class WatchdogConnection : public Connection::TcpConnection<WatchdogService::Operation> {
protected:
    void handleReceivedMessage(std::unique_ptr<Communication::Message<WatchdogService::Operation>> receivedMessage) override;
    void onTimerExpiration() override;

public:
    explicit WatchdogConnection(boost::asio::io_context& ioContext);
    virtual ~WatchdogConnection() override = default;
};

} // namespace Service