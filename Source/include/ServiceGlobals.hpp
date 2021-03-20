#pragma once
#include "Types.hpp"
#include <atomic>

namespace Service {
namespace Globals {
extern Types::ServiceIdentifier serviceIdentifier;
extern std::atomic<bool> isRunning;
} // namespace Globals
} // namespace Service