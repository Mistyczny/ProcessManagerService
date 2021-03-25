#include "Logging.hpp"
#include "ServiceBase.hpp"

int main() {
    int returnCode = 0;
    Log::initialize("ProcessManagerService", Log::LogLevel::TRACE);
    Service::Base serviceBase{};
    if (!serviceBase.readConfiguration()) {
        returnCode = -1;
    } else {
        serviceBase.initializeSockets();
        serviceBase.readAll();
        serviceBase.joinAll();
        if (!serviceBase.connectToWatchdog()) {
            Log::critical("Failed to connect to watchdog");
        } else if (!serviceBase.waitForConnectResponse()) {
            Log::critical("Timed out waiting for connect response from watchdog");
        } else {
            serviceBase.initialize();
            returnCode = serviceBase.runServiceTask();
        }
    }

    return returnCode;
}