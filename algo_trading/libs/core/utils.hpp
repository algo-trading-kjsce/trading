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

#include <atomic>
#include <functional>
#include <memory>
#include <string>

namespace zmq
{
class message_t;
}

namespace spdlog
{
class logger;
}

namespace trading::core
{
enum class node_type;
enum class channel_type;

/**
 * @brief Get the node type for the executable
 *
 * @return node_type
 */
node_type get_node_type() noexcept;

/**
 * @brief Get the name of the current node
 *
 * @return std::string
 */
std::string get_node_name();

/**
 * @brief Get the logger for the current node
 *
 * @return spdlog::logger
 */
std::shared_ptr<spdlog::logger> get_logger();

/**
 * @brief Create a node and set node info for the executable
 *
 * @param i_node_type incoming node type
 * @param i_kill_flag kill flag for nodes to exit threads gracefully
 * @param i_create_messenger create message publisher for messenger to publish to user
 */
void create_node_set_info( node_type i_node_type,
                           std::reference_wrapper<std::atomic_bool> i_kill_flag,
                           bool i_create_messenger );

/**
 * @brief Publish messages to /trading/outgoing_messages channel
 *
 * @param i_str incoming message
 */
void send_message( const std::string& i_str );

/**
 * @brief handle interrupt signal
 *
 * @param i_sig incoming signal
 */
void sigint_handler( int i_sig );

zmq::message_t create_zmq_message( const std::string& i_str );

}  // namespace trading::core::
