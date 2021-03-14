#pragma once
#include "Event.hpp"
#include "ServiceContextWorker.hpp"
#include "ServiceMessageEventsCache.hpp"
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
    MessageEventsCache messageEventsCache{};
    Task task{};
    std::vector<ContextWorker> contextWorkers;
    WatchdogConnectionState watchdogConnectionState{};
    std::shared_ptr<WatchdogConnection> watchdogConnection{nullptr};
    ModulesServer modulesServer;

public:
    Base();
    virtual ~Base();

    bool connectToWatchdog();
    bool waitForConnectResponse();
    void joinAll();
    void readAll();

    void initialize();

    int runServiceTask();
};

} // namespace Service