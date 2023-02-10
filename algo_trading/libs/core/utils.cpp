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

#include "libs/core/utils.hpp"

#include "libs/core/channel_type.hpp"
#include "libs/core/node_info.hpp"
#include "libs/core/node_type.hpp"

#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <zmq_addon.hpp>

#include <optional>
#include <string>

using namespace trading::core;
using namespace std::literals::string_literals;

namespace
{
node_info info_{};
}

namespace trading::core
{
node_type get_node_type() noexcept
{
    return info_.type;
}

std::string get_node_name()
{
    return std::to_string( get_node_type() );
}

// rclcpp::Logger get_logger()
// {
//     return info_.logger.value_or( rclcpp::get_logger( "" ) );
// }

// std::shared_ptr<rclcpp::Node> create_node_set_info( const node_type i_type,
//                                                     std::reference_wrapper<std::atomic_bool> i_kill_flag,
//                                                     bool i_create_messenger )
// {
//     info_.type = i_type;

//     const auto node{ std::make_shared<rclcpp::Node>( get_node_name() ) };

//     //info_.logger.emplace( node->get_logger() );

//     if( i_create_messenger )
//     {
//         // info_.message_publisher = node->create_publisher<std_msgs::msg::String>(
//         //     std::to_string( ros::channel_type::outgoing_messages ).data(), 10_sz );
//     }

//     info_.kill_flag.emplace( i_kill_flag );

//     return node;
// }

void send_message( const std::string& )
{
    // if( info_.message_publisher != nullptr )
    // {
    //     std_msgs::msg::String msg;
    //     msg.data = i_msg;

    //     info_.message_publisher->publish( msg );
    // }
}

void sigint_handler( [[maybe_unused]] int i_sig )
{
    info_.kill_flag.value().get() = true;

    // info_.message_publisher.reset();
}

zmq::message_t create_zmq_message( const std::string& i_str )
{
    zmq::message_t msg{ i_str.size() };
    std::memcpy( msg.data(), i_str.data(), i_str.size() );

    return msg;
}

}  // namespace trading::core
