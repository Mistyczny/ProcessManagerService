#include "ServiceGlobals.hpp"
#include "ServiceTask.hpp"
#include <chrono>

namespace Service {

Task::Task() { Service::Globals::serviceIdentifier = 872415233; }
Task::~Task() {}
int Task::run() {
    while (1) {
        std::cout << "IN MAIN" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 1;
}

} // namespace Service