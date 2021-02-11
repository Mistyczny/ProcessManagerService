#include "ServiceConnection.hpp"

namespace Service {

WatchdogConnection::WatchdogConnection(boost::asio::io_context& ioContext)
    : Connection::TcpConnection<WatchdogService::Operation>(ioContext) {}

void WatchdogConnection::handleReceivedMessage(std::unique_ptr<Communication::Message<WatchdogService::Operation>> receivedMessage) {}

void WatchdogConnection::onTimerExpiration() {}

} // namespace Service