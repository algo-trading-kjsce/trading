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

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <string_view>

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
    spdlog::get( trading::core::get_node_name().c_str() ) \
        ->debug( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) );

#define LOG_INFO( ... ) \
    spdlog::get( trading::core::get_node_name().c_str() ) \
        ->info( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) );

#define LOG_WARN( ... ) \
    spdlog::get( trading::core::get_node_name().c_str() ) \
        ->warn( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) );

#define LOG_ERROR( ... ) \
    spdlog::get( trading::core::get_node_name().c_str() ) \
        ->error( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) );

#define LOG_FATAL( ... ) \
    spdlog::get( trading::core::get_node_name().c_str() ) \
        ->critical( "{}: {}: {}", find_logging_path( __FILE__ ), __LINE__, fmt::format( __VA_ARGS__ ) );


// Conditional Logging

#define LOG_DEBUG_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_DEBUG( __VA_ARGS__ ); \
    }

#define LOG_INFO_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_INFO( __VA_ARGS__ ); \
    }

#define LOG_WARN_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_WARN( __VA_ARGS__ ); \
    }

#define LOG_ERROR_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_ERROR( __VA_ARGS__ ); \
    }

#define LOG_FATAL_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_FATAL( __VA_ARGS__ ); \
    }

// Log and send message to user
#define LOG_DEBUG_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_DEBUG( str ); \
    trading::core::send_message( str );

#define LOG_INFO_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_INFO( str ); \
    trading::core::send_message( str );

#define LOG_WARN_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_WARN( str ); \
    trading::core::send_message( str );

#define LOG_ERROR_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_ERROR( str ); \
    trading::core::send_message( str );

#define LOG_FATAL_SEND_MESSAGE( ... ) \
    const auto str{ fmt::format( __VA_ARGS__ ) }; \
    LOG_FATAL( str ); \
    trading::core::send_message( str );

#define LOG_DEBUG_SEND_MESSAGE_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_DEBUG( fmt::format( __VA_ARGS__ ) ); \
        trading::core::send_message( fmt::format( __VA_ARGS__ ) ); \
    }

#define LOG_INFO_SEND_MESSAGE_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_INFO( fmt::format( __VA_ARGS__ ) ); \
        trading::core::send_message( fmt::format( __VA_ARGS__ ) ); \
    }

#define LOG_WARN_SEND_MESSAGE_IF( condition, ... ) \
    if( condition ) \
    { \
        LOG_WARN( fmt::format( __VA_ARGS__ ) ); \
        trading::core::send_message( fmt::format( __VA_ARGS__ ) ); \
    }

#define LOG_ERROR_SEND_MESSAGE_IF( condition, ... ) \
    if( condition ) \
    { \
        const auto str{ fmt::format( __VA_ARGS__ ) }; \
        LOG_ERROR( condition, __VA_ARGS__ ); \
        trading::core::send_message( str ); \
    }

#define LOG_FATAL_SEND_MESSAGE_IF( condition, ... ) \
    if( condition ) \
    { \
        const auto str{ fmt::format( __VA_ARGS__ ) }; \
        LOG_FATAL( condition, __VA_ARGS__ ); \
        trading::core::send_message( str ); \
    }

// Utility macros

#define LOG_INVALID_ENUM( enum ) LOG_WARN( "invalid enum: {}", enum );
