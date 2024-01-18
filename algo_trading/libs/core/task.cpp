/**
 * @file task.cpp
 * @author ashwinn76
 * @brief Definition of task class
 * @version 0.1
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/task.hpp"

#include "json_include.hpp"
#include "type_trait_utils.hpp"

namespace trading::core
{
task::task( const task_type i_type,
            const node_type i_node,
            const time_point i_timestamp,
            std::optional<ticker_info> i_ticker ) noexcept :
    m_type_{ i_type }, m_node_{ i_node }, m_timestamp_{ i_timestamp }, m_ticker_{ std::move( i_ticker ) }
{
}

task_type task::type() const noexcept
{
    return m_type_;
}

node_type task::node() const noexcept
{
    return m_node_;
}

time_point task::timestamp() const noexcept
{
    return m_timestamp_;
}

const std::optional<ticker_info>& task::ticker() const noexcept
{
    return m_ticker_;
}

void to_json( json& o_js, const task& i_task )
{
    o_js = { { "type", i_task.type() },
             { "node", i_task.node() },
             { "timestamp", i_task.timestamp().time_since_epoch().count() },
             { "tickers", i_task.ticker() } };
}

void from_json( const json& i_js, task& o_task )
{
    o_task = task{ i_js["type"].get<task_type>(),
                   i_js["node"].get<node_type>(),
                   time_point{ duration{ i_js["timestamp"].get<std::size_t>() } },
                   i_js["ticker"].get<std::optional<ticker_info>>() };
}

bool operator==( const task& i_lhs, const task& i_rhs ) noexcept
{
    return ( ( i_lhs.type() == i_rhs.type() ) && ( i_lhs.node() == i_rhs.node() ) &&
             ( i_lhs.timestamp() == i_rhs.timestamp() ) && ( i_lhs.ticker() == i_rhs.ticker() ) );
}

bool operator!=( const task& i_lhs, const task& i_rhs ) noexcept
{
    return !( i_lhs == i_rhs );
}
}  // namespace trading::core
