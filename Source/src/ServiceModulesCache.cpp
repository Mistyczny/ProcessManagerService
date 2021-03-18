#include "ServiceModulesCache.hpp"

namespace Service {

void ModulesCache::addModule(Types::ModuleIdentifier moduleIdentifier, boost::asio::ip::udp::endpoint address) {
    std::unique_lock lock{sharedMutex};
    auto iter =
        std::find_if(std::begin(modulesCache), std::end(modulesCache), [&](auto& module) { return module.first == moduleIdentifier; });
    if (iter != std::end(modulesCache)) {
        modulesCache.emplace(moduleIdentifier, address);
    }
}

boost::asio::ip::udp::endpoint ModulesCache::getModulesDestinations(Types::ModuleIdentifier moduleIdentifier) {
    std::shared_lock lock{sharedMutex};
    boost::asio::ip::udp::endpoint address{};
    auto iter =
        std::find_if(std::begin(modulesCache), std::end(modulesCache), [&](auto& module) { return module.first == moduleIdentifier; });
    if (iter != std::end(modulesCache)) {
        address = iter->second;
    }
    return address;
}

} // namespace Service