#include "GpsCoordinates.hpp"

void GpsCoordinates::setCoordinates(Types::ModuleIdentifier identifier, std::pair<int32_t, int32_t> coordinates) {
    std::unique_lock lock(mutex);
    // Set coordinates
    auto iter = std::find_if(std::begin(identifierToCoordinates), std::end(identifierToCoordinates),
                             [&](auto& record) { return record.first == identifier; });
    if (iter == std::end(identifierToCoordinates)) {
        identifierToCoordinates.emplace(identifier, std::pair(coordinates.first, coordinates.second));
    } else {
        auto& [x, y] = iter->second;
        auto& [newX, newY] = coordinates;
        x = coordinates.first;
        y = coordinates.second;
    }
}

std::pair<int32_t, int32_t> GpsCoordinates::getCoordinates(Types::ModuleIdentifier) {
    std::shared_lock lock(mutex);
    // Get coordinates
    return {};
}