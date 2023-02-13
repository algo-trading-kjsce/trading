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

#include "fs_include.hpp"
#include "type_trait_utils.hpp"
#include "wise_enum_include.hpp"

#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <zmq_addon.hpp>

#include <optional>
#include <string>

using namespace trading::core;
using namespace std::literals::string_literals;

namespace
{
std::optional<node_info> info_{};
}

namespace trading::core
{
node_type get_node_type() noexcept
{
    return info_.value().type;
}

std::string get_node_name()
{
    return std::to_string( get_node_type() );
}

std::shared_ptr<spdlog::logger> get_logger()
{
    return info_.value().logger;
}

void create_node_set_info( const node_type i_type,
                           std::reference_wrapper<std::atomic_bool> i_kill_flag,
                           [[maybe_unused]] bool i_create_messenger )
{
    const auto node_name{ std::to_string( i_type ) };
    auto log_file{ fs::path{ std::getenv( "LOGDIR" ) } / ( node_name + "_log.txt" ) };

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back( std::make_shared<spdlog::sinks::stdout_color_sink_mt>() );
    sinks.push_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>( log_file ) );

    auto combined_logger = std::make_shared<spdlog::logger>( node_name, begin( sinks ), end( sinks ) );
    // register it if you need to access it globally

    spdlog::register_logger( combined_logger );
    info_ = node_info{ i_type, combined_logger, i_kill_flag };
}

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
    info_.value().kill_flag.get() = true;

    // info_.message_publisher.reset();
}

zmq::message_t create_zmq_message( const std::string& i_str )
{
    zmq::message_t msg{ i_str.size() };
    std::memcpy( msg.data(), i_str.data(), i_str.size() );

    return msg;
}

}  // namespace trading::core
