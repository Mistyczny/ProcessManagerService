#pragma once
#include "Event.hpp"
#include "ServiceContextWorker.hpp"
#include "ServiceModulesServer.hpp"
#include "ServiceTask.hpp"
#include "ServiceWatchdogConnection.hpp"
#include "ServiceWatchdogConnectionState.hpp"
#include <boost/asio.hpp>
#include <map>
#include <thread>

namespace Service {

class Base {
private:
    std::vector<std::thread> ioContextThreads{};
    std::multimap<uint32_t, std::unique_ptr<EventInterface>> serviceEventsMap;
    Task task{};
    std::vector<ContextWorker> contextWorkers;
    WatchdogConnectionState watchdogConnectionState{};
    std::shared_ptr<WatchdogConnection> watchdogConnection{nullptr};
    ModulesServer modulesServer;

public:
    Base();
    virtual ~Base() = default;

    void connectWatchdog();
    void joinAll();
    void readAll();

    void initialize();
};

} // namespace Service