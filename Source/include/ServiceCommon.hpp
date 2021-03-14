#pragma once
#include "Types.hpp"
#include <boost/asio.hpp>

namespace Service {

struct Sender {
    Types::ModuleIdentifier senderIdentifier;
    boost::asio::ip::udp::endpoint senderEndpoint{};
};

} // namespace Service