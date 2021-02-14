#pragma once
#include <chrono>
#include <condition_variable>
#include <mutex>

namespace Service {

class WatchdogConnectionState {
public:
    enum class ConnectionState { NotConnected, Connected, Shutdown };

private:
    ConnectionState connectionState{ConnectionState::NotConnected};
    mutable std::mutex connectionStateLock;
    std::condition_variable connectionStateCondition;

    void setConnectionState(ConnectionState& connectionState);

public:
    WatchdogConnectionState() = default;
    WatchdogConnectionState(const WatchdogConnectionState&) = delete;
    WatchdogConnectionState(WatchdogConnectionState&&) = delete;
    WatchdogConnectionState& operator=(const WatchdogConnectionState&) = delete;
    WatchdogConnectionState& operator=(WatchdogConnectionState&&) = delete;
    ~WatchdogConnectionState() = default;

    ConnectionState timedWaitForConnection(std::chrono::duration<int> timestampInSeconds);
    ConnectionState waitForConnectionStateChange();
    void setConnectionStateAndNotify(ConnectionState connectionState);

    [[nodiscard]] ConnectionState getConnectionState() const;
};

} // namespace Service