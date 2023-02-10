/**
 * @file trade_request.cpp
 * @author ashwinn76
 * @brief Definition of trade_request functions
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/trade_request.hpp"

namespace trading::core
{
trade_request::trade_request( const time_point i_timestamp,
                              std::string i_ticker,
                              const trade_type i_type,
                              const double i_price ) noexcept :
    m_timestamp_{ i_timestamp }, m_ticker_{ std::move( i_ticker ) }, m_type_{ i_type }, m_price_{ i_price }
{
}

time_point trade_request::timestamp() const noexcept
{
    return m_timestamp_;
}

std::string trade_request::ticker() const noexcept
{
    return m_ticker_;
}

trade_type trade_request::type() const noexcept
{
    return m_type_;
}

double trade_request::price() const noexcept
{
    return m_price_;
}
}  // namespace trading::core
