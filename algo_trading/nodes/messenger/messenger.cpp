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

#include "modules/telegram_bot/telegram_bot.hpp"

#include "libs/core/node_type.hpp"
#include "libs/core/utils.hpp"
#include "libs/zmqlib/create_socket.hpp"

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

using namespace trading;
using namespace trading::core;
using namespace trading::messenger;

int main( int argc, char** argv )
{
    std::atomic_bool kill_flag{ false };
    create_node_set_info( node_type::messenger, std::ref( kill_flag ), false );
    std::signal( SIGINT, sigint_handler );

    zmq::context_t ctx;

    telegram_bot bot{ argc > 1 ? fs::path{ argv[1] } : fs::path{} };

    auto incoming_messages_socket{ trading::zeromq::create_simple_pub_socket(
        ctx, std::ref( kill_flag ), "tcp://127.0.0.1:45000", true, [&bot]() { return bot.pull(); } ) };

    auto outgoing_messages_socket{ trading::zeromq::create_simple_sub_socket(
        ctx, std::ref( kill_flag ), "tcp://127.0.0.1:47000", true, {}, [&bot]( const zmq::message_t& i_msg ) {
            bot.push( i_msg.to_string() );
        } ) };

    return 0;
}
