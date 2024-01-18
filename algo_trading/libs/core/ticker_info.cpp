/**
 * @file ticker_info.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ticker_info.hpp"

namespace trading::core
{

ticker_info::ticker_info( std::string i_ticker, const double i_price ) noexcept :
    m_ticker_{ std::move( i_ticker ) }, m_price_{ i_price }
{
}

std::string ticker_info::ticker() const noexcept
{
    return m_ticker_;
}

double ticker_info::price() const noexcept
{
    return m_price_;
}

void to_json( json& o_js, const ticker_info& i_ticker_info )
{
    o_js = { { "ticker", i_ticker_info.ticker() }, { "price", i_ticker_info.price() } };
}

void from_json( const json& i_js, ticker_info& o_ticker_info )
{
    o_ticker_info = ticker_info{ i_js["ticker"].get<std::string>(), i_js["price"].get<double>() };
}

bool operator==( const ticker_info& i_lhs, const ticker_info& i_rhs ) noexcept
{
    return ( i_lhs.ticker() == i_rhs.ticker() ) && ( i_lhs.price() == i_rhs.price() );
}

bool operator!=( const ticker_info& i_lhs, const ticker_info& i_rhs ) noexcept
{
    return !( i_lhs == i_rhs );
}

}
