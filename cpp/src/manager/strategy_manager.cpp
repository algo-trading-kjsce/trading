/**
 * @file strategy_manager.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-24
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <mutex>

#include "type_trait_utils.hpp"

#include "strategy_manager.hpp"
#include "trading_manager.hpp"

static auto strategy_mtx{ std::mutex{} };

namespace
{
class active_strategy_lock
{
private:
    std::lock_guard<std::mutex> m_lock;

public:
    active_strategy_lock() : m_lock{ strategy_mtx }
    {
    }
};
}

namespace trading
{
strategy_manager::strategy_manager( trading_manager& i_manager ) : m_manager{ i_manager }
{
    m_ptrs.at( static_cast<std::size_t>( trading_strategy::bullish_engulfing ) ) =
        std::make_shared<bullish_engulfing>( m_manager );
}

void strategy_manager::trade( const std::string& i_ticker, const stock_data& i_data )
{
    auto lk{ active_strategy_lock{} };

    if( auto o_transaction{ m_manager.find_transaction_data( i_ticker ) }; o_transaction.has_value() )
    {
        if( get_strategy( o_transaction.value().strategy )->sell( i_ticker ) )
        {
            m_manager.add_task( std::make_unique<sell_task>( m_manager, i_ticker, i_data.candles().back() ) );
        }
    }
    else
    {
        auto best_strategy{ trading_strategy::__LAST };
        auto pattern_match{ 0_i32 };

        for( auto&& strategy_ptr : m_ptrs )
        {
            auto new_pattern_match{ strategy_ptr->buy( i_ticker ) };

            if( new_pattern_match > pattern_match )
            {
                pattern_match = new_pattern_match;

                best_strategy = strategy_ptr->strategy_type();
            }
        }

        if( best_strategy != trading_strategy::__LAST )
        {
            m_manager.add_task(
                std::make_unique<buy_task>( m_manager, i_ticker, i_data.candles().back(), best_strategy ) );
        }
    }
}

strategy_ptr strategy_manager::get_strategy( trading_strategy i_strategy )
{
    return m_ptrs.at( static_cast<std::size_t>( i_strategy ) );
}
}
