/**
 * @file manager.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2023-02-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "libs/zmqlib/channel_type.hpp"

#include "wise_enum_include.hpp"

#include <spdlog/spdlog.h>
#include <zmq.hpp>

#include <atomic>
#include <unordered_map>

using namespace trading::zeromq;


int main( [[maybe_unused]] int argc, [[maybe_unused]] char** argv )
{
    std::unordered_map<channel_type, zmq::socket_t> channel_to_port_map;

    zmq::context_t ctx;
    for( auto [channel, _] : wise_enum::enumerators<channel_type>::range )
    {
        zmq::socket_t socket{ ctx, zmq::socket_type::pub };
        socket.bind( "tcp://*:*" );
        auto port{ socket.get( zmq::sockopt::last_endpoint ) };
    }
}
