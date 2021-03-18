#include "ServiceContextWorker.hpp"
#include "Logging.hpp"
#include "MongoDbEnvironment.hpp"
#include "MongoModulesCollection.hpp"
#include "ServiceCommon.hpp"
#include "ServiceGlobals.hpp"
#include "ServiceModule.pb.h"
#include <algorithm>

namespace Service {

ContextWorker::ContextWorker(std::vector<std::thread>& ioContextThreads, ModulesCache& modulesCache, MessageEventsCache& messageEventsCache,
                             SubscriptionEventsCache& subscriptionEventsCache)
    : ioContextThreads{ioContextThreads}, modulesCache{modulesCache}, messageEventsCache{messageEventsCache}, subscriptionEventsCache{
                                                                                                                  subscriptionEventsCache} {
    boost::asio::socket_base::reuse_address option(true);
    auto endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 12234);
    socket = std::make_unique<boost::asio::ip::udp::socket>(ioContext);
    socket->open(endpoint.protocol());
    socket->set_option(option);
    socket->bind(endpoint);
}

ContextWorker::ContextWorker(ContextWorker&& other) noexcept
    : ioContextThreads(std::move(other).ioContextThreads), modulesCache{std::move(other).modulesCache},
      messageEventsCache(std::move(other).messageEventsCache),
      subscriptionEventsCache{std::move(other).subscriptionEventsCache}, socket{std::move(other.socket)} {}

ContextWorker& ContextWorker::operator=(ContextWorker&& other) noexcept {
    this->socket = std::move(other.socket);
    this->ioContextThreads = std::move(other.ioContextThreads);
    return *this;
}

void ContextWorker::runAll() {
    for (auto threadNr = 0; threadNr < 5; threadNr++) {
        ioContextThreads.emplace_back([&]() { ioContext.run(); });
    }
}

void ContextWorker::startRead() {
    if (socket) {
        socket->async_receive_from(
            boost::asio::buffer(messageBuffer), remoteEndpoint, [this](const boost::system::error_code& error, std::size_t bytesRead) {
                if (error) {
                    this->handleReadError(error);
                } else {
                    // Copy received message to local message buffer
                    std::string receivedMessageBuffer{};
                    receivedMessageBuffer.reserve(bytesRead);
                    std::copy_n(messageBuffer.begin(), bytesRead, std::back_inserter(receivedMessageBuffer));

                    // Create sender structure
                    Sender sender{};
                    sender.senderEndpoint = remoteEndpoint;

                    // Now we have local copies of all required things, we can start listening again
                    this->startRead();

                    ServiceModule::Message receivedMessage{};
                    if (receivedMessage.ParseFromString(receivedMessageBuffer)) {
                        sender.senderIdentifier = receivedMessage.header().senderidentifier();
                        const auto operationCode = receivedMessage.header().operationcode();
                        if (operationCode == ServiceModule::ModuleRequest) {
                            auto responses = messageEventsCache.triggerMessageHandlers(sender, receivedMessage.request().request());

                            std::for_each(std::begin(responses), std::end(responses), [&](auto& response) {
                                ServiceModule::Message responseMessage{};
                                auto* responseHeader = new ServiceModule::Header{};
                                responseHeader->set_senderidentifier(Service::Globals::serviceIdentifier);
                                responseHeader->set_operationcode(ServiceModule::OperationCode::ServiceResponse);
                                responseHeader->set_transactioncode(receivedMessage.header().transactioncode());
                                responseMessage.set_allocated_header(responseHeader);
                                auto* responseRequest = new ServiceModule::Request{};
                                responseRequest->CopyFrom(response);
                                std::string message{};
                                responseMessage.SerializeToString(&message);
                                this->socket->async_send_to(boost::asio::buffer(message), sender.senderEndpoint,
                                                            [](const boost::system::error_code& error, std::size_t bytesRead) {
                                                                if (error) {
                                                                    std::cout << "SENDING ERROR" << std::endl;
                                                                } else {
                                                                    std::cout << "SENDING SUCCESS" << std::endl;
                                                                }
                                                            });
                            });
                            this->handleSubscriptions(receivedMessage.request().request());
                        }
                    } else {
                        Log::error("Failed to parse message from string");
                    }
                }
            });
    }
}

void ContextWorker::handleSubscriptions(google::protobuf::Any anyMessage) {
    auto responses = this->subscriptionEventsCache.triggerMessageHandlers(anyMessage);
    if (!responses.empty()) {
        auto modulesCollectionEntry = Mongo::DbEnvironment::getInstance()->getClient();
        Mongo::ModulesCollection modulesCollection{*modulesCollectionEntry, "Modules"};

        std::for_each(std::begin(responses), std::end(responses), [&](auto& response) {
            auto destinationModule = modulesCollection.getModule(response.first);
            if (destinationModule.has_value()) {
                ServiceModule::Message responseMessage{};
                auto* responseHeader = new ServiceModule::Header{};
                responseHeader->set_senderidentifier(Service::Globals::serviceIdentifier);
                responseHeader->set_operationcode(ServiceModule::OperationCode::ServiceRequest);
                responseHeader->set_transactioncode(19);
                responseMessage.set_allocated_header(responseHeader);
                auto* responseRequest = new ServiceModule::Request{};
                responseRequest->CopyFrom(response.second);
                std::string message{};
                responseMessage.SerializeToString(&message);

                boost::asio::ip::address address = boost::asio::ip::address::from_string(destinationModule->ipAddress);
                boost::asio::ip::udp::endpoint clientEndpoint{address, destinationModule->port};
                this->socket->async_send_to(boost::asio::buffer(message), clientEndpoint,
                                            [](const boost::system::error_code& error, std::size_t bytesRead) {
                                                if (error) {
                                                    std::cout << "SENDING ERROR" << std::endl;
                                                } else {
                                                    std::cout << "SENDING SUCCESS" << std::endl;
                                                }
                                            });
            }
        });
    }
}

void ContextWorker::handleReadError(const boost::system::error_code& error) {}

void ContextWorker::stopIoContext() { ioContext.stop(); }

} // namespace Service