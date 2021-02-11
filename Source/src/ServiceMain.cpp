#include "ServiceBase.hpp"

int main() {
    Service::Base serviceBase{};
    serviceBase.connectWatchdog();
    serviceBase.readAll();
    serviceBase.joinAll();
}