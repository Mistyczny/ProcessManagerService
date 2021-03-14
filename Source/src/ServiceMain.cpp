#include "Logging.hpp"
#include "ServiceBase.hpp"
#include <iostream>

int main() {
    Log::initialize("1234", Log::LogLevel::TRACE);
    Service::Base serviceBase{};
    std::cout << "A" << std::endl;
    serviceBase.readAll();
    std::cout << "B" << std::endl;
    serviceBase.joinAll();
    std::cout << "C" << std::endl;
    if (!serviceBase.connectToWatchdog()) {
        Log::critical("Failed to connect to watchdog");
    } else if (!serviceBase.waitForConnectResponse()) {
        Log::critical("Failed to connect to watchdog");
    } else {
        std::cout << "ABC" << std::endl;
        serviceBase.runServiceTask();
        std::cout << "ABCD" << std::endl;
    }
}