/**
 * @file candle.hpp
 * @author ashwinn76
 * @brief Declaration of candle
 * @version 0.1
 * @date 2022-08-09
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "clock.hpp"
#include "json_include.hpp"

#include <string>

namespace trading::core
{
class candle
{
public:
    /**
     * @brief Construct a new candle object
     *
     */
    explicit candle() = default;

    /**
     * @brief Construct a new candle object
     *
     * @param i_ticker ticker to which this candle belongs
     * @param i_timestamp time stamp of candle
     * @param i_open opening price
     * @param i_high highest price
     * @param i_low lowest price
     * @param i_close closing price
     */
    explicit candle( std::string i_ticker,
                     time_point i_timestamp,
                     double i_open,
                     double i_high,
                     double i_low,
                     double i_close ) noexcept;

    /**
     * @brief Get the ticker
     *
     * @return std::string
     */
    std::string ticker() const noexcept;

    /**
     * @brief Get the timestamp of the candle
     *
     * @return time_point
     */
    time_point timestamp() const noexcept;

    /**
     * @brief Get the opening price of the candle
     *
     * @return double
     */
    double opening_price() const noexcept;

    /**
     * @brief Get the lowest price
     *
     * @return double
     */
    double lowest_price() const noexcept;

    /**
     * @brief Get the highest price
     *
     * @return double
     */
    double highest_price() const noexcept;

    /**
     * @brief Get the closing price of the candle
     *
     * @return double
     */
    double closing_price() const noexcept;

private:
    std::string m_ticker_{};

    time_point m_timestamp_{};

    double m_open_{ 0.0 };
    double m_high_{ 0.0 };
    double m_low_{ 0.0 };
    double m_close_{ 0.0 };
};

void to_json( json& o_js, const candle& i_candle );
void from_json( const json& i_js, candle& o_candle );

bool operator==( const candle& i_lhs, const candle& i_rhs ) noexcept;
bool operator!=( const candle& i_lhs, const candle& i_rhs ) noexcept;

}  // namespace trading::core
