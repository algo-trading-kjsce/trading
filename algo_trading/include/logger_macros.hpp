/**
 * @file logger.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2022-07-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "libs/core/utils.hpp"

#include <rclcpp/rclcpp.hpp>

#include <fmt/format.h>

#include <string_view>
#include <string>

inline auto find_logging_path( const std::string_view& i_path )
{
    using namespace std::literals::string_view_literals;

    static const std::vector directories{ "libs"sv, "modules"sv, "nodes"sv };

    for( auto&& directory : directories )
    {
        if( const auto dir_pos{ i_path.find( directory ) }; dir_pos != std::string_view::npos )
        {
            return std::string{ i_path.substr( dir_pos ) };
        }
    }

    return std::string{ i_path };
}

#define LOG_DEBUG( ... ) \
    RCLCPP_DEBUG( \
        trading::core::ros::get_logger(), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_INFO( ... ) \
    RCLCPP_INFO( \
        trading::core::ros::get_logger(), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_WARN( ... ) \
    RCLCPP_WARN( \
        trading::core::ros::get_logger(), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_ERROR( ... ) \
    RCLCPP_ERROR( \
        trading::core::ros::get_logger(), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_FATAL( ... ) \
    RCLCPP_FATAL( \
        trading::core::ros::get_logger(), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );


// Conditional Logging

#define LOG_DEBUG_IF( condition, ... ) \
    RCLCPP_DEBUG_EXPRESSION( \
        trading::core::ros::get_logger(), \
        ( condition ), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_INFO_IF( condition, ... ) \
    RCLCPP_INFO_EXPRESSION( \
        trading::core::ros::get_logger(), \
        ( condition ), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_WARN_IF( condition, ... ) \
    RCLCPP_WARN_EXPRESSION( \
        trading::core::ros::get_logger(), \
        ( condition ), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_ERROR_IF( condition, ... ) \
    RCLCPP_ERROR_EXPRESSION( \
        trading::core::ros::get_logger(), \
        ( condition ), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

#define LOG_FATAL_IF( condition, ... ) \
    RCLCPP_FATAL_EXPRESSION( \
        trading::core::ros::get_logger(), \
        ( condition ), \
        fmt::format( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) ).c_str() );

// Log and send message to user
#define LOG_DEBUG_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_DEBUG( str ); \
    trading::core::ros::send_message( str );

#define LOG_INFO_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_INFO( str ); \
    trading::core::ros::send_message( str );

#define LOG_WARN_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_WARN( str ); \
    trading::core::ros::send_message( str );

#define LOG_ERROR_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_ERROR( str ); \
    trading::core::ros::send_message( str );

#define LOG_FATAL_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_FATAL( str ); \
    trading::core::ros::send_message( str );

#define LOG_DEBUG_SEND_MESSAGE_IF( condition, ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_DEBUG_IF( condition, __VA_ARGS__ ); \
    trading::core::ros::send_message( str );

#define LOG_INFO_SEND_MESSAGE_IF( condition, ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_INFO_IF( condition, __VA_ARGS__ ); \
    trading::core::ros::send_message( str );

#define LOG_WARN_SEND_MESSAGE_IF( condition, ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_WARN_IF( condition, __VA_ARGS__ ); \
    trading::core::ros::send_message( str );

#define LOG_ERROR_SEND_MESSAGE_IF( condition, ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_ERROR_IF( condition, __VA_ARGS__ ); \
    trading::core::ros::send_message( str );

#define LOG_FATAL_SEND_MESSAGE_IF( condition, ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_FATAL_IF( condition, __VA_ARGS__ ); \
    trading::core::ros::send_message( str );

// Utility macros

#define LOG_INVALID_ENUM( enum ) LOG_WARN( "invalid enum: {}", enum );
