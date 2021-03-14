#include "ServiceContextWorker.hpp"
#include "Logging.hpp"
#include "ServiceCommon.hpp"
#include "ServiceGlobals.hpp"
#include "ServiceModule.pb.h"
#include <algorithm>

namespace Service {

ContextWorker::ContextWorker(std::vector<std::thread>& ioContextThreads, MessageEventsCache& messageEventsCache)
    : ioContextThreads{ioContextThreads}, messageEventsCache{messageEventsCache} {
    boost::asio::socket_base::reuse_address option(true);
    auto endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 12234);
    socket = std::make_unique<boost::asio::ip::udp::socket>(ioContext);
    socket->open(endpoint.protocol());
    socket->set_option(option);
    socket->bind(endpoint);
}

ContextWorker::ContextWorker(ContextWorker&& other) noexcept
    : ioContextThreads(std::move(other).ioContextThreads),
      messageEventsCache(std::move(other).messageEventsCache), socket{std::move(other.socket)} {}

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
                        }
                    } else {
                        Log::error("Failed to parse message from string");
                    }
                }
            });
    }
}

void ContextWorker::handleReadError(const boost::system::error_code& error) {}

void ContextWorker::stopIoContext() { ioContext.stop(); }

} // namespace Service