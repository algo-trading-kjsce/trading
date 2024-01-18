/**
 * @file create_socket.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2023-02-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "libs/zmqlib/utils.hpp"

#include "string_include.hpp"

#include <zmq.hpp>

#include <atomic>
#include <functional>
#include <future>
#include <memory>

namespace trading::zeromq
{

template<typename function_t>
std::future<void> create_simple_pub_socket( zmq::context_t& i_ctx,
                                            std::reference_wrapper<std::atomic_bool> i_kill_flag,
                                            const std::string& i_port,
                                            const bool i_bind,
                                            function_t i_func )
{
    using func_type = typename std::invoke_result_t<function_t>::value_type;
    static_assert( std::convertible_to<std::string, func_type> || std::convertible_to<json, func_type>,
                   "Object cannot be converted into a string" );

    auto socket{ std::make_unique<zmq::socket_t>( i_ctx, zmq::socket_type::pub ) };
    i_bind ? socket->bind( i_port ) : socket->connect( i_port );

    return std::async(
        std::launch::async,
        [socket = std::move( socket ), kill_flag = i_kill_flag, func = std::forward<function_t>( i_func )]() {
            while( !kill_flag.get() )
            {
                for( auto&& msg : func() )
                {
                    socket->send( trading::zeromq::create_zmq_message( std::to_string( msg ) ), zmq::send_flags::none );
                }
            }
        } );
}

template<typename function_t>
std::future<void> create_simple_sub_socket( zmq::context_t& i_ctx,
                                            std::reference_wrapper<std::atomic_bool> i_kill_flag,
                                            const std::string& i_port,
                                            const bool i_bind,
                                            std::vector<std::string> subscriptions,
                                            function_t&& i_func )
{
    auto socket{ std::make_unique<zmq::socket_t>( i_ctx, zmq::socket_type::sub ) };
    i_bind ? socket->bind( i_port ) : socket->connect( i_port );

    if( subscriptions.empty() )
    {
        socket->set( zmq::sockopt::subscribe, "" );
    }

    for( auto&& topic : subscriptions )
    {
        socket->set( zmq::sockopt::subscribe, topic );
    }

    return std::async(
        std::launch::async,
        [socket = std::move( socket ), kill_flag = i_kill_flag, func = std::forward<function_t>( i_func )]() {
            while( !kill_flag.get() )
            {
                zmq::message_t incoming_message;
                const auto res{ socket->recv( incoming_message, zmq::recv_flags::dontwait ) };

                if( res.has_value() )
                {
                    func( incoming_message );
                }
            }
        } );
}

}
