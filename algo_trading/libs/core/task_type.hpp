/**
 * @file task_type.hpp
 * @author ashwinn76
 * @brief Declaration of task_type enumeration
 * @version 0.1
 * @date 2022-07-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "json_include.hpp"
#include "string_include.hpp"
#include "wise_enum_include.hpp"

namespace trading::core
{
WISE_ENUM_CLASS( task_type, unknown, start, stop, buy, sell, status, kill )

NLOHMANN_JSON_SERIALIZE_ENUM( task_type,
                              { { task_type::unknown, nullptr },
                                { task_type::start, std::to_string( task_type::start ) },
                                { task_type::stop, std::to_string( task_type::stop ) },
                                { task_type::buy, std::to_string( task_type::buy ) },
                                { task_type::sell, std::to_string( task_type::sell ) },
                                { task_type::status, std::to_string( task_type::status ) },
                                { task_type::kill, std::to_string( task_type::kill ) } } )

constexpr auto is_node_level_task( const task_type i_type )
{
    if( i_type == task_type::kill || i_type == task_type::status )
    {
        return true;
    }
    return false;
}

constexpr auto is_ticker_task( const task_type i_type )
{
    if( i_type == task_type::buy || i_type == task_type::sell || i_type == task_type::start ||
        i_type == task_type::stop )
    {
        return true;
    }
    return false;
}

constexpr auto does_task_need_price_point( const task_type i_type )
{
    if( i_type == task_type::buy || i_type == task_type::sell )
    {
        return true;
    }
    return false;
}

}  // namespace trading::core
