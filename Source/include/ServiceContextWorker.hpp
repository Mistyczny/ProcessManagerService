#pragma once
#include "Event.hpp"
#include "ServiceMessageEventsCache.hpp"
#include "ServiceModule.pb.h"
#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <map>
#include <thread>

namespace Service {

class ContextWorker {
private:
    boost::asio::io_context ioContext{};
    std::vector<std::thread>& ioContextThreads;
    MessageEventsCache& messageEventsCache;
    std::unique_ptr<boost::asio::ip::udp::socket> socket{nullptr};
    std::string messageBuffer{};

    void handleReadError(const boost::system::error_code& error);

public:
    explicit ContextWorker(std::vector<std::thread>& ioContextThreads, MessageEventsCache& messageEventsCache);
    ContextWorker(const ContextWorker& other) = delete;
    ContextWorker(ContextWorker&& other) noexcept;
    ContextWorker& operator=(const ContextWorker& other) = delete;
    ContextWorker& operator=(ContextWorker&& other) noexcept;
    virtual ~ContextWorker() = default;

    constexpr boost::asio::io_context& getContext() { return ioContext; }

    void startRead();
    void runAll();
    void stopIoContext();
};

} // namespace Service