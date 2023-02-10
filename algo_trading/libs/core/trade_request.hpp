/**
 * @file trade_request.hpp
 * @author ashwinn76
 * @brief Declaration of trade_request
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "libs/core/trade_type.hpp"

#include "clock.hpp"

#include <string>

namespace trading::core
{
/**
 * @brief Trade request created by planner
 *
 */
class trade_request
{
private:
    time_point m_timestamp_{};
    std::string m_ticker_{};

    trade_type m_type_{};

    double m_price_{};

public:
    /**
     * @brief Construct a new trade request object
     *
     * @param i_timestamp time stamp of request
     * @param i_ticker ticker of trade request
     * @param i_type type of trade
     * @param i_price price at which trade needs to be executed
     */
    trade_request( time_point i_timestamp, std::string i_ticker, trade_type i_type, double i_price ) noexcept;

    /**
     * @brief Get the timestamp of trade request
     *
     * @return time_point
     */
    time_point timestamp() const noexcept;

    /**
     * @brief Get the ticker for which this trade request is set
     *
     * @return std::string
     */
    std::string ticker() const noexcept;

    /**
     * @brief Get the type of trade
     *
     * @return trade_type
     */
    trade_type type() const noexcept;

    /**
     * @brief Get the price of trade request
     *
     * @return double
     */
    double price() const noexcept;
};
}  // namespace trading::core
