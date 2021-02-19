#include "ServiceBase.hpp"
#include <iostream>

int main() {
    Service::Base serviceBase{};
    if (!serviceBase.connectToWatchdog()) {
        Log::critical("Failed to connect to watchdog");
    } else if (!serviceBase.waitForConnectResponse()) {
        Log::critical("Failed to connect to watchdog");
    } else {
        serviceBase.readAll();
        serviceBase.joinAll();
    }
}