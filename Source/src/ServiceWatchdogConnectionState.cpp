#include "ServiceWatchdogConnectionState.hpp"

namespace Service {

WatchdogConnectionState::ConnectionState WatchdogConnectionState::timedWaitForConnection(std::chrono::duration<int> timestampInSeconds) {
    std::unique_lock lock(this->connectionStateLock);
    this->connectionStateCondition.wait_for(lock, timestampInSeconds);
    return this->connectionState;
}

WatchdogConnectionState::ConnectionState WatchdogConnectionState::waitForConnectionStateChange() {
    std::unique_lock lock(this->connectionStateLock);
    this->connectionStateCondition.wait(lock);
    return this->connectionState;
}

void WatchdogConnectionState::setConnectionState(ConnectionState& connectionState) {
    std::unique_lock lock(this->connectionStateLock);
    this->connectionState = connectionState;
}

void WatchdogConnectionState::setConnectionStateAndNotify(ConnectionState connectionState) {
    this->setConnectionState(connectionState);
    connectionStateCondition.notify_one();
}

WatchdogConnectionState::ConnectionState WatchdogConnectionState::getConnectionState() const {
    std::unique_lock lock(this->connectionStateLock);
    return this->connectionState;
}

} // namespace Service