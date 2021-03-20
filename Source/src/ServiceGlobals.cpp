#include "ServiceGlobals.hpp"

namespace Service {
namespace Globals {
Types::ServiceIdentifier serviceIdentifier;
std::atomic<bool> isRunning{true};
} // namespace Globals
} // namespace Service