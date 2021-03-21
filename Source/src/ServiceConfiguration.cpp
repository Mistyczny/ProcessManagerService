#include "ServiceConfiguration.hpp"
#include "Logging.hpp"

namespace Service {

bool ServerConfiguration::readFromJson(const nlohmann::json& jsonConfig) {
    bool configurationRead{true};
    try {
        maxMessageResends = jsonConfig["ServerConfiguration"]["MaxMessageResends"].get<uint32_t>();
        timeBetweenMessageResendsInSeconds = jsonConfig["ServerConfiguration"]["TimeBetweenMessageResendsInSeconds"].get<uint32_t>();
        messageBufferSize = jsonConfig["ServerConfiguration"]["MessageBufferSize"].get<uint32_t>();
        listeningPort = jsonConfig["ServerConfiguration"]["ListeningPort"].get<uint16_t>();
    } catch (nlohmann::json::exception& ex) {
        Log::error("Failed to read configuration: " + std::string(ex.what()));
        configurationRead = false;
    }
    return configurationRead;
}

void ServerConfiguration::print() {
    Log::trace("ServerConfiguration::maxMessageResends = " + std::to_string(this->maxMessageResends));
    Log::trace("ServerConfiguration::timeBetweenMessageResendsInSeconds = " + std::to_string(this->timeBetweenMessageResendsInSeconds));
    Log::trace("ServerConfiguration::listeningPort = " + std::to_string(this->listeningPort));
}

bool LoggingConfiguration::readFromJson(const nlohmann::json& jsonConfig) {
    bool configurationRead{true};
    try {
        loggingLevel = jsonConfig["LoggingConfiguration"]["Level"].get<std::string>();
    } catch (nlohmann::json::exception& ex) {
        Log::error("Failed to read configuration: " + std::string(ex.what()));
        configurationRead = false;
    }
    return configurationRead;
}

ServerConfiguration& Configuration::getServerConfiguration() { return serverConfiguration; }
LoggingConfiguration& Configuration::getLoggingConfiguration() { return loggingConfiguration; }

} // namespace Service