/**
 * @file trade_context.cpp
 * @author ashwinn76
 * @brief Definition of trade_context
 * @version 0.1
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "trade_block.hpp"

#include "logger_macros.hpp"

namespace trading::planner
{
trade_block::trade_block( std::string i_ticker ) noexcept : m_ticker_{ std::move( i_ticker ) }
{
}

bool trade_block::should_keep_processing() const noexcept
{
    return m_keep_processing_;
}

void trade_block::add_candle( core::candle i_candle )
{
    {
        std::lock_guard _{ m_mtx_ };

        m_candles_.push_back( std::move( i_candle ) );
    }

    m_candles_processed_ = false;

    LOG_INFO( "Found new candles for {}", m_ticker_ );
}

void trade_block::remove_candle()
{
    std::lock_guard _{ m_mtx_ };

    m_candles_.pop_front();
}

bool trade_block::has_new_candles() const noexcept
{
    return m_candles_processed_;
}

void trade_block::set_candles_processed() noexcept
{
    m_candles_processed_ = true;

    LOG_INFO( "Finished processing candles for {}", m_ticker_ );
}

price_points trade_block::get_price_points() const
{
    std::lock_guard _{ m_mtx_ };

    price_points res;

    res.open.reserve( m_candles_.size() );
    res.high.reserve( m_candles_.size() );
    res.close.reserve( m_candles_.size() );
    res.low.reserve( m_candles_.size() );

    for( auto&& candle : m_candles_ )
    {
        res.open.push_back( candle.opening_price() );
        res.high.push_back( candle.highest_price() );
        res.close.push_back( candle.closing_price() );
        res.low.push_back( candle.lowest_price() );
    }

    return res;
}

std::string trade_block::get_ticker() const noexcept
{
    return m_ticker_;
}
}  // namespace trading::planner
