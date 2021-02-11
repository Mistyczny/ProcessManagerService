#include "ServiceModulesServer.hpp"

namespace Service {

ModulesServer::ModulesServer(std::vector<ContextWorker>& contextWorkers) : contextWorkers{contextWorkers} {}

void ModulesServer::startReadingAll() {
    for (auto& work : contextWorkers) {
        work.startRead();
    }
}

} // namespace Service