#pragma once
#include "Types.hpp"
#include <boost/asio.hpp>
#include <mongocxx/client.hpp>
#include <optional>

namespace Mongo {

struct ModuleRecord {
    uint16_t port;
    Types::ModuleIdentifier identifier;
    int32_t connectionState;
    std::string ipAddress;
};

class ModulesCollection {
private:
    mongocxx::collection modulesCollection;
    std::optional<ModuleRecord> viewToModuleRecord(bsoncxx::document::view&);

public:
    ModulesCollection(mongocxx::client& client, std::string collectionName);
    virtual ~ModulesCollection() = default;

    std::optional<ModuleRecord> getModule(const Types::ModuleIdentifier& moduleIdentifier);
};

} // namespace Mongo