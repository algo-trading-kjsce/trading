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

#include "modules/task_translator/task_translator.hpp"

#include "libs/core/node_type.hpp"
#include "libs/core/task.hpp"
#include "libs/core/utils.hpp"
#include "libs/zmqlib/create_socket.hpp"

#include "json_include.hpp"
#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <zmq.hpp>

#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <future>
#include <string>

using namespace trading;
using namespace trading::core;
using namespace trading::translator;

int main()
{
    std::atomic_bool kill_flag{ false };
    create_node_set_info( node_type::translator, std::ref( kill_flag ), false );
    std::signal( SIGINT, sigint_handler );

    zmq::context_t ctx;

    translator::task_translator translator;

    auto incoming_messages_socket{ zeromq::create_simple_sub_socket(
        ctx, std::ref( kill_flag ), "tcp://127.0.0.1:45000", false, {}, [&translator]( const zmq::message_t& i_msg ) {
            translator.translate( i_msg.to_string() );
        } ) };

    // auto task_creation_thread{ std::async(
    //     std::launch::async, send_outgoing_tasks, std::ref( ctx ), std::ref( translator ) ) };

    auto outgoing_messages_socket{ trading::zeromq::create_simple_pub_socket(
        ctx, std::ref( kill_flag ), "tcp://127.0.0.1:47000", true, [&translator]() {
            return translator.retrieve_tasks();
        } ) };

    incoming_messages_socket.wait();
    // task_creation_thread.wait();

    return 0;
}
