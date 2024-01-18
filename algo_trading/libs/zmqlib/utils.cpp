/**
 * @file node_utils.cpp
 * @author ashwinn76
 * @brief Definition of node helper functions
 * @version 0.1
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/zmqlib/utils.hpp"

#include <zmq.hpp>

#include <string>

using namespace trading::zeromq;
using namespace std::literals::string_literals;

namespace trading::zeromq
{

zmq::message_t create_zmq_message( const std::string& i_str )
{
    zmq::message_t msg{ i_str.size() };
    std::memcpy( msg.data(), i_str.data(), i_str.size() );

    return msg;
}

}  // namespace trading::zmq
