/**
 * @file candle.cpp
 * @author ashwinn76
 * @brief Definition of candle functions
 * @version 0.1
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/candle.hpp"

#include "json_include.hpp"

#include <string>
#include <utility>

namespace trading::core
{
candle::candle( std::string i_ticker,
                time_point i_timestamp,
                double i_open,
                double i_high,
                double i_low,
                double i_close ) noexcept :
    m_ticker_{ std::move( i_ticker ) },
    m_timestamp_{ i_timestamp },
    m_open_{ i_open },
    m_high_{ i_high },
    m_low_{ i_low },
    m_close_{ i_close }
{
}

std::string candle::ticker() const noexcept
{
    return m_ticker_;
}

time_point candle::timestamp() const noexcept
{
    return m_timestamp_;
}

double candle::opening_price() const noexcept
{
    return m_open_;
}

double candle::closing_price() const noexcept
{
    return m_close_;
}

double candle::highest_price() const noexcept
{
    return m_high_;
}

double candle::lowest_price() const noexcept
{
    return m_low_;
}

void to_json( json& o_js, const candle& i_candle )
{
    o_js = { { "ticker", i_candle.ticker() },      { "timestamp", i_candle.timestamp().time_since_epoch().count() },
             { "open", i_candle.opening_price() }, { "high", i_candle.highest_price() },
             { "low", i_candle.lowest_price() },   { "close", i_candle.closing_price() } };
}

void from_json( const json& i_js, candle& o_candle )
{
    o_candle =
        candle{ i_js["ticker"].get<std::string>(), time_point{ duration{ i_js["timestamp"].get<std::size_t>() } },
                i_js["open"].get<double>(),        i_js["high"].get<double>(),
                i_js["low"].get<double>(),         i_js["close"].get<double>() };
}

bool operator==( const candle& i_lhs, const candle& i_rhs ) noexcept
{
    return ( i_lhs.ticker() == i_rhs.ticker() ) && ( i_lhs.timestamp() == i_rhs.timestamp() ) &&
           ( i_lhs.opening_price() == i_rhs.opening_price() ) && ( i_lhs.highest_price() == i_lhs.highest_price() ) &&
           ( i_lhs.lowest_price() == i_rhs.lowest_price() ) && ( i_lhs.closing_price() == i_rhs.closing_price() );
}

bool operator!=( const candle& i_lhs, const candle& i_rhs ) noexcept
{
    return !( i_lhs == i_rhs );
}

}  // namespace trading::core
