#include "Service.hpp"
#include "ServiceGlobals.hpp"
#include "ServiceTask.hpp"

namespace Service {

Task::Task() { Service::Globals::serviceIdentifier = 872415233; }
Task::~Task() {}
int Task::run() { return Service::ReturnCodes::RUN_FOREVER; }

} // namespace Service