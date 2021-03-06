/**
 * @file robinhood_bot.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include "stock_data.hpp"

#include "helper/py_object.hpp"

namespace trading
{
class robinhood_bot
{
private:
    python::py_object m_bot{};  // Robinhood Bot
    python::py_object m_historyFunc{};  // Function to get history of prices
    python::py_object m_last_price_func{};  // Function to get latest price

public:
    /**
     * @brief Construct a new robinhood bot object
     *
     */
    robinhood_bot();

    /**
     * @brief Get the historical prices for a stock/currency
     *
     * @param i_symbol Symbol to find info
     * @return csv_data Final historical result
     */
    csv_data get_historical_prices( const std::string& i_symbol );

    /**
     * @brief Get the latest price for a stock/currency
     *
     * @param i_symbol Symbol to find info
     * @return candle_s Candle for latest price
     */
    candle_s get_latest_price( const std::string& i_symbol );

    /**
     * @brief Buy a ticker
     *
     * @param i_ticker incoming ticker
     * @return number of shares and price per share bought
     */
    std::pair<double, double> buy( const std::string& i_ticker );

    /**
     * @brief sell a ticker
     *
     * @param i_ticker incoming ticker
     * @return sale price of ticker
     */
    double sell( const std::string& i_ticker );
};
}
