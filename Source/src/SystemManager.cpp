#include "SystemManager.hpp"
#include "MongoDbEnvironment.hpp"

SystemManager::SystemManager() {
    auto entry = Mongo::DbEnvironment::getInstance()->getClient();
    this->usersCollection = std::make_unique<Mongo::UsersCollection>(*entry, "Users");
}

SystemManager::~SystemManager() {}

void SystemManager::run() {}