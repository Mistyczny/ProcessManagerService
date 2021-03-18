#pragma once
#include "Event.hpp"
#include "ServiceMessageEventsCache.hpp"
#include "ServiceModule.pb.h"
#include "ServiceModulesCache.hpp"
#include "ServiceSubscriptionEventsCache.hpp"
#include <array>
#include <boost/asio.hpp>
#include <cstdint>
#include <map>
#include <thread>

namespace Service {

class ContextWorker {
private:
    boost::asio::io_context ioContext{};
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_worker{boost::asio::make_work_guard(ioContext)};
    std::vector<std::thread>& ioContextThreads;
    ModulesCache& modulesCache;
    MessageEventsCache& messageEventsCache;
    SubscriptionEventsCache& subscriptionEventsCache;
    std::unique_ptr<boost::asio::ip::udp::socket> socket{nullptr};
    std::string messageBuffer{};
    boost::asio::ip::udp::endpoint remoteEndpoint{};

    void handleReadError(const boost::system::error_code& error);
    void handleSubscriptions(google::protobuf::Any);

public:
    explicit ContextWorker(std::vector<std::thread>&, ModulesCache&, MessageEventsCache&, SubscriptionEventsCache&);
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