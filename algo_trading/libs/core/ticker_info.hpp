/**
 * @file ticker_info.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "json_include.hpp"

#include <string>

namespace trading::core
{

class ticker_info
{
public:
    /**
     * @brief Construct a new ticker info object
     *
     */
    ticker_info() noexcept = default;

    /**
     * @brief Construct a new ticker info object
     *
     * @param i_ticker incoming ticker
     * @param i_price incoming price
     */
    ticker_info( std::string i_ticker, double i_price ) noexcept;

    /**
     * @brief get ticker
     *
     * @return std::string
     */
    std::string ticker() const noexcept;

    /**
     * @brief get price
     *
     * @return double
     */
    double price() const noexcept;

private:
    std::string m_ticker_{};
    double m_price_{ 0.0 };
};

void to_json( json& o_js, const ticker_info& i_ticker_info );
void from_json( const json& i_js, ticker_info& o_ticker_info );

bool operator==( const ticker_info& i_lhs, const ticker_info& i_rhs ) noexcept;
bool operator!=( const ticker_info& i_lhs, const ticker_info& i_rhs ) noexcept;
}
