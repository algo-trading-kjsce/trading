/**
 * @file planner.cpp
 * @author ashwinn76
 * @brief Declaration of planner node
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "modules/trade_planner/trade_planner.hpp"

#include "libs/core/node_type.hpp"
#include "libs/core/task.hpp"
#include "libs/core/utils.hpp"
#include "libs/zmqlib/create_socket.hpp"

#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <zmq.hpp>

#include <csignal>
#include <functional>

using namespace trading;
using namespace trading::core;
using namespace trading::planner;

std::vector<std::string> parse_tickers( int i_argc, char** i_argv )
{
    std::vector<std::string> tickers;
    tickers.reserve( i_argc - 1 );

    for( int i = 1; i < i_argc; ++i )
    {
        tickers.push_back( i_argv[i] );
    }

    return tickers;
}

int main( int argc, char** argv )
{
    std::atomic_bool kill_flag{ false };
    create_node_set_info( node_type::planner, std::ref( kill_flag ), false );
    std::signal( SIGINT, sigint_handler );

    zmq::context_t ctx;

    trade_planner planner{ std::ref( kill_flag ), parse_tickers( argc, argv ) };

    auto incoming_messages_socket{ trading::zeromq::create_simple_pub_socket(
        ctx, std::ref( kill_flag ), "tcp://127.0.0.1:45000", true, [&planner]() {
            return planner.retrieve_tasks();
        } ) };

    auto outgoing_messages_socket{ trading::zeromq::create_simple_sub_socket(
        ctx, std::ref( kill_flag ), "tcp://127.0.0.1:47000", true, {}, [&planner]( const zmq::message_t& i_msg ) {
            planner.process_task( std::make_json( i_msg.to_string() ).get<task>() );
        } ) };

    return 0;
}
