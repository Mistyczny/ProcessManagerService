#include "ServiceBase.hpp"
#include "EventManager.hpp"

namespace Service {

Base::Base() : modulesServer{contextWorkers} {
    contextWorkers.reserve(2);
    for (int i = 0; i < 2; i++) {
        contextWorkers.emplace_back(ioContextThreads, serviceEventsMap);
    }
    watchdogConnection = std::make_shared<WatchdogConnection>(contextWorkers.front().getContext(), watchdogConnectionState);
}

void Base::joinAll() {
    for (auto& contextWorker : contextWorkers) {
        contextWorker.runAll();
    }
    std::for_each(std::begin(ioContextThreads), std::end(ioContextThreads), std::mem_fn(&std::thread::join));
}

void Base::connectWatchdog() {
    boost::asio::ip::tcp::endpoint endpoint{boost::asio::ip::address::from_string("127.0.0.1"), 1235};
    if (watchdogConnection->connect(endpoint)) {
        watchdogConnection->startReading();
    }
}

void Base::readAll() { modulesServer.startReadingAll(); }

void Base::initialize() { EventManager::initialize(this->serviceEventsMap); }

} // namespace Service