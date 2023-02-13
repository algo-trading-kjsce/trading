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

#include "fs_include.hpp"
#include "logger_macros.hpp"
#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <spdlog/spdlog.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>

using namespace trading;
using namespace trading::core;
using namespace trading::messenger;

using namespace std::literals::chrono_literals;

std::atomic_bool kill_flag{ false };

void run_message_listener( zmq::context_t& i_ctx, telegram_bot& o_bot )
{
    zmq::socket_t incoming_message_socket{ i_ctx, zmq::socket_type::pub };
    incoming_message_socket.bind( "tcp://127.0.0.1:45000" );
    std::this_thread::sleep_for( 1s );
    LOG_INFO( "Done" );

    while( !kill_flag )
    {
        const auto incoming_messages{ o_bot.pull() };
        for( auto&& message : incoming_messages )
        {
            LOG_INFO( "Got message: {}", message );
            incoming_message_socket.send( create_zmq_message( message ), zmq::send_flags::none );
        }

        std::this_thread::sleep_for( 100ms );
    }
}


int main( int argc, char** argv )
{
    std::cout << __LINE__ << std::endl;
    create_node_set_info( node_type::messenger, std::ref( kill_flag ), false );
    std::cout << __LINE__ << std::endl;
    std::signal( SIGINT, sigint_handler );
    std::cout << __LINE__ << std::endl;

    zmq::context_t ctx;
    std::cout << __LINE__ << std::endl;

    telegram_bot bot{ argc > 1 ? fs::path{ argv[1] } : fs::path{} };
    std::cout << __LINE__ << std::endl;

    auto listener_thread{ std::async( std::launch::async, run_message_listener, std::ref( ctx ), std::ref( bot ) ) };
    std::cout << __LINE__ << std::endl;

    listener_thread.wait();
    std::cout << __LINE__ << std::endl;

    return 0;
}
