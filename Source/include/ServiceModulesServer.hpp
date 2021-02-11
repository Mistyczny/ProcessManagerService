#pragma once
#include "ServiceContextWorker.hpp"
#include <list>
namespace Service {

class ModulesServer {
private:
    std::vector<ContextWorker>& contextWorkers;

public:
    explicit ModulesServer(std::vector<ContextWorker>& contextWorkers);
    virtual ~ModulesServer() = default;

    void startReadingAll();
};

} // namespace Service