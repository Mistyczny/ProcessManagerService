#pragma once
#include <cstdint>
#include <nlohmann/json.hpp>

namespace Service {

struct ServerConfiguration {
    uint32_t maxMessageResends;
    uint32_t timeBetweenMessageResendsInSeconds;
    uint32_t messageBufferSize;
    uint32_t listeningPort;

    bool readFromJson(const nlohmann::json& jsonConfig);
    void print();
};

struct LoggingConfiguration {
    std::string loggingLevel;

    bool readFromJson(const nlohmann::json& jsonConfig);
    void print();
};

class Configuration {
private:
    ServerConfiguration serverConfiguration{};
    LoggingConfiguration loggingConfiguration{};

public:
    Configuration() = default;
    virtual ~Configuration() = default;

    ServerConfiguration& getServerConfiguration();
    LoggingConfiguration& getLoggingConfiguration();

    static Configuration& getInstance() {
        static Configuration instance;
        return instance;
    }
};

} // namespace Module