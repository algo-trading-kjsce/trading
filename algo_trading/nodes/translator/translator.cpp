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

#include "modules/translator/translator.hpp"

#include "libs/core/channel_type.hpp"
#include "libs/core/node_type.hpp"
#include "libs/core/task.hpp"
#include "libs/core/utils.hpp"

#include "json_include.hpp"
#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include <atomic>
#include <chrono>
#include <csignal>
#include <functional>
#include <future>
#include <iostream>
#include <string>

using namespace trading;
using namespace trading::core;
using namespace trading::translator;

using namespace std::literals::chrono_literals;

std::atomic_bool kill_flag{ false };

void process_incoming_messages( zmq::context_t& i_ctx, task_translator& io_translator )
{
    zmq::socket_t incoming_messages_socket{ i_ctx, zmq::socket_type::sub };
    incoming_messages_socket.connect( "tcp://127.0.0.1:45000" );
    incoming_messages_socket.set( zmq::sockopt::subscribe, "" );

    while( !kill_flag )
    {
        zmq::message_t incoming_message;
        const auto res{ incoming_messages_socket.recv( incoming_message ) };

        if( res.has_value() )
        {
            std::cout << "Translator received: " << incoming_message.to_string() << std::endl;
            io_translator.translate( incoming_message.str() );
        }
    }
}

void send_outgoing_tasks( zmq::context_t& i_ctx, task_translator& io_translator )
{
    zmq::socket_t tasks_socket( i_ctx, zmq::socket_type::pub );
    tasks_socket.bind( "tcp://127.0.0.1:46000" );

    while( true )
    {
        const auto tasks{ io_translator.retrieve_tasks() };
        for( auto&& task : tasks )
        {
            const auto task_str{ std::make_json( task ).dump() };
            std::cout << "sending task: " << task_str << std::endl;

            tasks_socket.send( create_zmq_message( task_str ), zmq::send_flags::none );
        }
    }
}

int main()
{
    create_node_set_info( node_type::messenger, std::ref( kill_flag ), false );
    std::signal( SIGINT, sigint_handler );

    zmq::context_t ctx;

    translator::task_translator translator;

    auto incoming_message_thread{ std::async(
        std::launch::async, process_incoming_messages, std::ref( ctx ), std::ref( translator ) ) };

    // auto task_creation_thread{ std::async(
    //     std::launch::async, send_outgoing_tasks, std::ref( ctx ), std::ref( translator ) ) };

    incoming_message_thread.wait();
    // task_creation_thread.wait();

    return 0;
}
