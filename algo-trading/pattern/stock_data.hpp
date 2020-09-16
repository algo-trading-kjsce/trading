/**
 * @file stock_data.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2020-09-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <list>
#include <tuple>
#include <vector>

#include "structs.hpp"

 /**
  * @brief A collection of daily stock fluctuations
  *
  */
class stock_data
{
private:
    date_s m_date{};

    std::list<candle_s> m_candles{};

    std::list<std::vector<double>> m_custom_columns{};
    std::list<std::vector<int>> m_strategy_columns{};

public:
    using stock_iterator = typename decltype(m_candles)::const_iterator;

    /**
     * @brief Construct a new CStockData object
     *
     * @param date Date of stock information
     */
    explicit stock_data(date_s date) noexcept;


    /**
     * @brief Add new candle stick to stock information
     *
     * @param i_candleStick candle stick to add
     */
    void add_candle(candle_s i_candle) noexcept;


    /**
     * @brief Add new custom value to the stock data
     *
     * @param i_custom_column custom value to add
     */
    void add_custom_column(std::vector<double> i_custom_column) noexcept;
    
    
    /**
     * @brief Add new custom value to the stock data
     *
     * @param i_custom_column custom value to add
     */
    void add_strategy_column(std::vector<int> i_strategy_column) noexcept;


    /**
     * @brief begin iterator
     *
     * @return begin iterator for CStockData object
     */
    stock_iterator begin() const noexcept;


    /**
     * @brief end iterator
     *
     * @return beyond-the-end iterator for CStockData object
     */
    stock_iterator end() const noexcept;


    /**
     * @brief Get column-wise data for the day
     *
     * @return open, high, low and close values for the day
     */
    raw_stock_input_s raw_values() const noexcept;


    /**
     * @brief Return the newly added column values in csv format
     *
     * @param i_index row index
     *
     * @return std::string
     */
    std::string columns_str(size_t i_index) const noexcept;
};
