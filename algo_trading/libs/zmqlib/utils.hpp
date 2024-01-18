/**
 * @file node_utils.hpp
 * @author ashwinn76
 * @brief Declaration of node helper functions
 * @version 0.1
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <string>

namespace zmq
{
class message_t;
}

namespace trading::zeromq
{

/**
 * @brief Create a zmq message object
 *
 * @param i_str incoming string
 * @return zmq::message_t
 */
zmq::message_t create_zmq_message( const std::string& i_str );

}  // namespace trading::zmq
