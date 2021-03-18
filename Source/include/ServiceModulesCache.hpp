#pragma once
#include "Types.hpp"
#include <boost/asio.hpp>
#include <map>
#include <shared_mutex>

namespace Service {

class ModulesCache {
private:
    mutable std::shared_mutex sharedMutex;
    std::map<Types::ModuleIdentifier, boost::asio::ip::udp::endpoint> modulesCache;

public:
    ModulesCache() = default;
    virtual ~ModulesCache() = default;

    void addModule(Types::ModuleIdentifier, boost::asio::ip::udp::endpoint);
    boost::asio::ip::udp::endpoint getModulesDestinations(Types::ModuleIdentifier);
};

} // namespace Service