#include "ServiceContextWorker.hpp"
#include "Logging.hpp"
#include "ServiceModule.pb.h"
#include <boost/bind.hpp>
#include <iostream>

namespace Service {

ContextWorker::ContextWorker(std::vector<std::thread>& ioContextThreads,
                             std::multimap<uint32_t, std::unique_ptr<EventInterface>>& serviceEventsMap)
    : ioContextThreads{ioContextThreads}, serviceEventsMap{serviceEventsMap} {
    boost::asio::socket_base::reuse_address option(true);
    auto endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 12234);
    socket = std::make_unique<boost::asio::ip::udp::socket>(ioContext);
    socket->open(endpoint.protocol());
    socket->set_option(option);
    socket->bind(endpoint);
}

ContextWorker::ContextWorker(ContextWorker&& other) noexcept
    : ioContextThreads(std::move(other).ioContextThreads),
      serviceEventsMap(std::move(other).serviceEventsMap), socket{std::move(other.socket)} {}

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
        socket->async_receive(boost::asio::buffer(messageBuffer), [this](const boost::system::error_code& error, std::size_t bytesRead) {
            if (error) {
                this->handleReadError(error);
            } else {
                std::string receivedMessageBuffer{};
                receivedMessageBuffer.reserve(bytesRead);
                std::copy_n(messageBuffer.begin(), bytesRead, std::back_inserter(receivedMessageBuffer));
                this->startRead();

                ServiceModule::Request receivedRequest{};
                if (receivedRequest.ParseFromString(receivedMessageBuffer)) {
                    const auto operationCode = receivedRequest.header().operationcode();
                    auto iter = serviceEventsMap.equal_range(operationCode);
                    for (auto& eventHandler = iter.first; eventHandler != iter.second; eventHandler++) {
                        // Run all registered handlers
                        eventHandler->second->handleReceivedMessage(receivedMessageBuffer);
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