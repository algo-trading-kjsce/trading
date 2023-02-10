/**
 * @file messenger.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2022-07-18
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "modules/messenger/messenger.hpp"

#include "libs/core/channel_type.hpp"
#include "libs/core/node_type.hpp"
#include "libs/core/utils.hpp"

#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <chrono>
#include <csignal>
#include <functional>
#include <future>
#include <iostream>

using namespace trading;
using namespace trading::core;
using namespace trading::messenger;

using namespace std::literals::chrono_literals;

void run_message_listener( zmq::context_t& i_ctx, telegram_bot& o_bot )
{
    zmq::socket_t incoming_message_socket{ i_ctx, zmq::socket_type::pub };
    incoming_message_socket.bind( "tcp://127.0.0.1:45000" );
    std::this_thread::sleep_for( 1s );
    std::cout << "Done" << std::endl;

    while( true )
    {
        const auto incoming_messages{ o_bot.pull() };
        for( auto&& message : incoming_messages )
        {
            std::cout << "Got message: " << message << std::endl;
            auto msg{ create_zmq_message( message ) };
            std::cout << msg << std::endl;
            incoming_message_socket.send( msg, zmq::send_flags::none );
        }

        std::this_thread::sleep_for( 100ms );
    }
}


int main( int argc, char** argv )
{
    zmq::context_t ctx;

    telegram_bot bot{ argc > 1 ? fs::path{ argv[1] } : fs::path{} };

    auto listener_thread{ std::async( std::launch::async, run_message_listener, std::ref( ctx ), std::ref( bot ) ) };

    listener_thread.wait();

    return 0;
}
