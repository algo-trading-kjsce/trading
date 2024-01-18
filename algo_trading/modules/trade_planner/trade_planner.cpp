/**
 * @file planner.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "modules/trade_planner/trade_planner.hpp"

#include "libs/core/task.hpp"
#include "libs/core/task_type.hpp"

namespace trading::planner
{
trade_planner::trade_planner( std::reference_wrapper<std::atomic_bool> i_kill_flag,
                              std::vector<std::string> i_tickers ) :
    m_kill_flag_{ i_kill_flag }
{
    for( auto&& ticker : i_tickers )
    {
        m_trade_blocks_.emplace( ticker, std::make_shared<trade_block>( ticker ) );

        m_trade_threads_.emplace(
            ticker,
            std::async( std::launch::async, [&kill_flag = m_kill_flag_, context = m_trade_blocks_.at( ticker )]() {
                while( context->should_keep_processing() && !kill_flag.get() )
                {
                    if( context->has_new_candles() )
                    {
                        [[maybe_unused]] const auto prices{ context->get_price_points() };
                    }
                }
            } ) );
    }
}

std::vector<core::task> trade_planner::retrieve_tasks()
{
    std::lock_guard _{ m_task_mtx_ };

    auto res{ std::move( m_trade_requests_ ) };

    return res;
}

void trade_planner::process_task( core::task i_task )
{
    switch( i_task.type() )
    {
    case core::task_type::stop:
        /* code */
        break;

    default:
        break;
    }
}

}  // namespace trading::planner
