#include "MongoModulesCollection.hpp"
#include "Logging.hpp"
#include "MongoDbEnvironment.hpp"
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

namespace Mongo {

ModulesCollection::ModulesCollection(mongocxx::client& client, std::string collectionName)
    : modulesCollection{client["ProcessManager"][collectionName]} {}

std::optional<ModuleRecord> ModulesCollection::viewToModuleRecord(bsoncxx::document::view& view) {
    std::optional<ModuleRecord> moduleRecord{std::nullopt};
    auto modIdentifier = view["ModuleIdentifier"];
    auto connectionState = view["ConnectionState"];
    auto ipAddress = view["IpAddress"];
    auto port = view["Port"];

    if (modIdentifier.type() != bsoncxx::type::k_int32) {
        Log::error("Failed to get module identifier");
    } else if (connectionState.type() != bsoncxx::type::k_int32) {
        Log::error("Failed to get connection state");
    } else if (ipAddress.type() != bsoncxx::type::k_utf8) {
        Log::error("Failed to get ip address");
    } else if (port.type() != bsoncxx::type::k_int32) {
        Log::error("Failed to get port");
    } else {
        moduleRecord = std::make_optional<ModuleRecord>();
        moduleRecord->identifier = modIdentifier.get_int32();
        int32_t connState = connectionState.get_int32();
        moduleRecord->connectionState = static_cast<int32_t>(connState);
        auto adressView = ipAddress.get_string().value;
        moduleRecord->ipAddress = adressView.to_string();
        moduleRecord->port = port.get_int32();
    }
    return moduleRecord;
}

std::optional<ModuleRecord> ModulesCollection::getModule(const Types::ModuleIdentifier& moduleIdentifier) {
    std::optional<ModuleRecord> moduleRecord{std::nullopt};
    auto builder = document{};
    bsoncxx::document::value entry = builder << "ModuleIdentifier" << moduleIdentifier << finalize;

    auto result = modulesCollection.find_one(std::move(entry));
    if (result) {
        bsoncxx::document::view view{result->view()};
        moduleRecord = this->viewToModuleRecord(view);
    }
    return moduleRecord;
}

} // namespace Mongo