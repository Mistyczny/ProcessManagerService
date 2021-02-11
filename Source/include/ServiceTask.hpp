#pragma once
#include "Event.hpp"
#include "EventManager.hpp"
#include <map>
#include <memory>

namespace Service {

class Task {
private:
public:
    Task();
    virtual ~Task();
    int run();
};

} // namespace Service