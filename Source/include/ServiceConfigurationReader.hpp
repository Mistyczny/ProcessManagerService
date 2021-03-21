#pragma once
#include "ServiceConfiguration.hpp"
#include "Types.hpp"
#include <nlohmann/json.hpp>

namespace Service {

class ConfigurationReader {
private:
    Configuration& configuration;

    std::string configurationDirectoryPath{"/opt/ProcessManager/ServicesConfigurations/"};
    nlohmann::json configJson{};

    bool readConfig();

public:
    explicit ConfigurationReader(Configuration& configuration);

    bool readConfiguration(Types::ServiceIdentifier identifier);
};

} // namespace Service