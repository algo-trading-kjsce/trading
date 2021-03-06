/**
 * @file tasks.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>

#include "processor/utilities.hpp"

#include "tasks.hpp"
#include "trading_manager.hpp"

namespace trading
{
task_base::task_base( trading_manager& i_manager, trading_task_type i_task_type ) noexcept :
    m_manager{ i_manager }, m_type{ i_task_type }
{
}


trading_task_type task_base::get_type() const noexcept
{
    return m_type;
}


trading_manager& task_base::get_manager() noexcept
{
    return m_manager;
}


abort_task::abort_task( trading_manager& i_manager ) noexcept : task_base{ i_manager, trading_task_type::termination }
{
}


void abort_task::execute()
{
    get_manager().m_keep_running = false;

    get_manager().clear_tasks();
}


trade_task::trade_task( trading_manager& i_manager,
                        const std::string& i_ticker,
                        const candle_s& i_candle,
                        trading_task_type i_task_type ) noexcept :
    task_base{ i_manager, i_task_type }, m_candle{ i_candle }, m_ticker{ i_ticker }
{
}


buy_task::buy_task( trading_manager& i_manager,
                    const std::string& i_ticker,
                    const candle_s& i_candle,
                    trading_strategy i_strategy ) noexcept :
    trade_task{ i_manager, i_ticker, i_candle, trading_task_type::buy }, m_strategy{ i_strategy }
{
}


void buy_task::execute()
{
    auto [n_shares, price] = get_manager().get_robinhood_bot().buy( m_ticker );

    get_manager().add_trade( m_ticker, m_strategy, std::move( m_candle ), n_shares, price );
}


sell_task::sell_task( trading_manager& i_manager, const std::string& i_ticker, const candle_s& i_candle ) noexcept :
    trade_task{ i_manager, i_ticker, i_candle, trading_task_type::sell }
{
}


void sell_task::execute()
{
    auto price{ get_manager().get_robinhood_bot().sell( m_ticker ) };

    get_manager().finish_trade( m_ticker, std::move( m_candle ), price );
}


unknown_cmd::unknown_cmd( trading_manager& i_manager, std::string i_cmd ) noexcept :
    task_base{ i_manager, trading_task_type::unknown }, m_cmd{ std::move( i_cmd ) }
{
}


void unknown_cmd::execute()
{
    trading::utilities::async_cout( "Unknown command: ", m_cmd, '\n' );
}
}
