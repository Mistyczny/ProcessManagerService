#include "ServiceBase.hpp"
#include "EventManager.hpp"
#include <thread>

namespace Service {

Base::Base() : modulesServer{contextWorkers} {
    for (int i = 0; i < 1; i++) {
        contextWorkers.emplace_back(ioContextThreads, serviceEventsMap);
    }
    watchdogConnection = std::make_shared<WatchdogConnection>(contextWorkers.front().getContext(), watchdogConnectionState);
}

Base::~Base() {
    std::for_each(std::begin(contextWorkers), std::end(contextWorkers), [](auto& ioContextWorker) { ioContextWorker.stopIoContext(); });
    std::for_each(std::begin(ioContextThreads), std::end(ioContextThreads), [](auto& ioContextThread) {
        if (ioContextThread.joinable()) {
            ioContextThread.join();
        }
    });
}

void Base::joinAll() {
    std::for_each(std::begin(contextWorkers), std::end(contextWorkers), [](auto& contextWorker) { contextWorker.runAll(); });
}

bool Base::connectToWatchdog() {
    bool connectedToWatchdog{false};
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::address::from_string("127.0.0.1"), 1235};
    if (watchdogConnection->connect(endpoint)) {
        watchdogConnection->sendConnectRequest();
        watchdogConnection->startReading();
        connectedToWatchdog = true;
    }
    return connectedToWatchdog;
}

bool Base::waitForConnectResponse() {
    bool waitForConnectResponse{false};
    auto connectionState = watchdogConnectionState.timedWaitForConnection(std::chrono::seconds(10));
    if (connectionState == WatchdogConnectionState::ConnectionState::Connected) {
        this->watchdogConnection->setTimerExpiration(1000);
        waitForConnectResponse = true;
    }
    return waitForConnectResponse;
}

void Base::readAll() { modulesServer.startReadingAll(); }

void Base::initialize() { EventManager::initialize(this->serviceEventsMap); }

} // namespace Service