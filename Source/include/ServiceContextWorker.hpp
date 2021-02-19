#pragma once
#include "Event.hpp"
#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <map>
#include <thread>

namespace Service {

class ContextWorker {
private:
    uint32_t servedClientsCount{0};
    boost::asio::io_context ioContext{};
    std::vector<std::thread>& ioContextThreads;
    std::multimap<uint32_t, std::unique_ptr<EventInterface>>& serviceEventsMap;
    std::unique_ptr<boost::asio::ip::udp::socket> socket{nullptr};
    boost::asio::ip::udp::endpoint clientEndpoint{};
    std::array<char, 1024> buffer{};
    std::string str{};

public:
    explicit ContextWorker(std::vector<std::thread>& ioContextThreads, std::multimap<uint32_t, std::unique_ptr<EventInterface>>&);
    ContextWorker(const ContextWorker& other) = delete;
    ContextWorker(ContextWorker&& other) noexcept;
    ContextWorker& operator=(const ContextWorker& other) = delete;
    ContextWorker& operator=(ContextWorker&& other) noexcept;
    virtual ~ContextWorker() = default;

    constexpr boost::asio::io_context& getContext() { return ioContext; }

    void startRead();
    void runAll();
    void handle_receive(const boost::system::error_code& error, std::size_t);
    void stopIoContext();
};

} // namespace Service