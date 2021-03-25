#pragma once
#include "Types.hpp"
#include "shared_mutex"
#include <map>

class GpsCoordinates {
private:
    mutable std::shared_mutex mutex;
    std::map<Types::ModuleIdentifier, std::pair<int32_t, int32_t>> identifierToCoordinates;

public:
    GpsCoordinates() = default;
    ~GpsCoordinates() = default;

    void setCoordinates(Types::ModuleIdentifier, std::pair<int32_t, int32_t>);
    std::pair<int32_t, int32_t> getCoordinates(Types::ModuleIdentifier);
};