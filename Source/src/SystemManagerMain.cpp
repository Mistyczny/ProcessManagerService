#include "MongoDbEnvironment.hpp"
#include "SystemManager.hpp"
#include <iostream>

int main() {
    Mongo::DbEnvironment::initialize("127.0.0.1");
    if (!Mongo::DbEnvironment::isConnected()) {
        std::cout << "Failed connection to mongoDB" << std::endl;
    } else {
        SystemManager systemManager{};
        systemManager.run();
    }
}