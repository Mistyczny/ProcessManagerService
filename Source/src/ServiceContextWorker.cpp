#include "ServiceContextWorker.hpp"
#include <boost/bind.hpp>
#include <iostream>

namespace Service {

ContextWorker::ContextWorker(std::vector<std::thread>& ioContextThreads,
                             std::multimap<uint32_t, std::unique_ptr<EventInterface>>& serviceEventsMap)
    : ioContextThreads{ioContextThreads}, serviceEventsMap{serviceEventsMap} {
    boost::asio::socket_base::reuse_address option(true);
    auto endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 13);
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
    memset(&buffer, 0, buffer.size());
    str.resize(1024);
    if (socket) {
        socket->async_receive(boost::asio::buffer(str), boost::bind(&ContextWorker::handle_receive, this, boost::asio::placeholders::error,
                                                                    boost::asio::placeholders::bytes_transferred));
    }
}

void ContextWorker::handle_receive(const boost::system::error_code& error, std::size_t) {
    if (error) {
        return;
    }
    std::string receivedMessage{std::begin(buffer), std::end(buffer)};
    this->startRead();
    // Add handle
    int receivedKey = 1;
    auto iter = serviceEventsMap.equal_range(receivedKey);
    for (auto& itt = iter.first; itt != iter.second; itt++) {
        itt->second->handleReceivedMessage(receivedMessage);
    }
}

void ContextWorker::stopIoContext() { ioContext.stop(); }

} // namespace Service