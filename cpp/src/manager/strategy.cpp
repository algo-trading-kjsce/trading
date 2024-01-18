/**
 * @file base_strategy.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-24
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "ta-lib/ta_func.h"

#include "strategy.hpp"
#include "trading_manager.hpp"


namespace
{
static const auto function_map =
    std::unordered_map<trading_strategy, void*>{ { trading_strategy::bullish_engulfing, (void*)TA_CDLENGULFING } };

template<typename... _Args>
auto run_buy( trading_strategy i_strategy, _Args&&... i_args )
{
    if( auto iter{ function_map.find( i_strategy ) }; iter != function_map.end() )
    {
        using func_t = TA_RetCode ( * )( _Args... );

        if( auto func_ptr{ reinterpret_cast<func_t>( iter->second ) }; func_ptr != nullptr )
        {
            return func_ptr( std::forward<_Args>( i_args )... );
        }
    }

    throw std::runtime_error{ "function not found for strategy!" };
}
}

namespace trading
{
base_strategy::base_strategy( trading_manager& i_manager, trading_strategy i_strategy ) noexcept :
    m_manager{ i_manager }, m_strategy{ i_strategy }
{
}


trading_strategy base_strategy::strategy_type() const noexcept
{
    return m_strategy;
}

bullish_engulfing::bullish_engulfing( trading_manager& i_manager ) noexcept :
    base_strategy{ i_manager, trading_strategy::bullish_engulfing }
{
}


std::int32_t bullish_engulfing::buy( const std::string& i_ticker )
{
    if( auto& stock{ m_manager.find_stock_data( i_ticker ) }; !stock.candles().empty() )
    {
        auto n_entries{ stock.candles().size() };

        auto raw_values{ stock.raw_values() };

        auto begin_idx{ 0_i32 };
        auto n_elems{ 0_i32 };

        auto patterns{ std::vector<std::int32_t>( n_entries, 0_i32 ) };

        run_buy( trading_strategy::bullish_engulfing,
                 0_i32,
                 static_cast<std::int32_t>( n_entries ) - 1_i32,
                 raw_values.opens.data(),
                 raw_values.highs.data(),
                 raw_values.lows.data(),
                 raw_values.closes.data(),
                 std::addressof( begin_idx ),
                 std::addressof( n_elems ),
                 patterns.data() );

        return patterns.back();
    }

    return 0_i32;
}

bool bullish_engulfing::sell( const std::string& i_ticker )
{
    if( auto& stock{ m_manager.find_stock_data( i_ticker ) }; !stock.candles().empty() )
    {
        if( auto o_transaction{ m_manager.find_transaction_data( i_ticker ) }; o_transaction.has_value() )
        {
            auto last_candle{ stock.candles().rbegin() };
            auto second_last_candle{ std::next( last_candle ) };

            if( ( last_candle->time - o_transaction->buy->candle.time ) > 0 )
            {
                auto avg{ ( second_last_candle->open + second_last_candle->close ) / 2.0 };

                return last_candle->direction() == candle_direction::down && last_candle->close < avg;
            }
        }
    }

    return false;
}
}
