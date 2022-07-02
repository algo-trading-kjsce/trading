/**
 * @file stock_data.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include "common/enums.hpp"
#include "common/fs_include.hpp"
#include "common/structs.hpp"

#include "candle.hpp"
#include "date.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>
#include <vector>

/**
 * @brief A collection of daily stock fluctuations
 *
 */
class stock_data
{
private:
    date_s m_date{};  // date of the stock data

    std::int32_t m_candle_size{};  // candle size in minutes

    std::list<candle_s> m_candles{};  // List of candles of that particular stock data

    // List of probable strategies identified by TA-Lib
    std::unordered_map<ta_helper::ta_strategy, std::vector<std::int32_t>> m_strategies{};

public:
using candles = decltype(m_candles);
    using iterator = typename candles::iterator;
    using const_iterator = typename candles::const_iterator;

    /**
     * @brief Construct a new CStockData object
     *
     * @param i_candle_size Size of candle
     * @param i_date Date of stock information
     */
    explicit stock_data( std::int32_t i_candle_size, date_s i_date ) noexcept;

    /**
     * @brief Add new candle stick to stock information
     *
     * @param i_candleStick candle stick to add
     *
     * @return true if candle was added
     */
    bool add_candle( candle_s i_candle ) noexcept;

    /**
     * @brief Add new strategy pattern to the stock data
     *
     * @param i_strategy type of strategy
     * @param i_custom_column custom value to add
     */
    void add_strategy_column( ta_helper::ta_strategy i_strategy, std::vector<std::int32_t> i_strategy_column ) noexcept;

    /**
     * @brief begin iterator
     *
     * @return begin iterator for stock_data object
     */
    iterator begin() noexcept;

    /**
     * @brief end iterator
     *
     * @return beyond-the-end iterator for stock_data object
     */
    iterator end() noexcept;

    /**
     * @brief const begin iterator
     *
     * @return begin iterator for stock_data object
     */
    const_iterator begin() const noexcept;

    /**
     * @brief const end iterator
     *
     * @return beyond-the-end iterator for stock_data object
     */
    const_iterator end() const noexcept;

    /**
     * @brief get access to candlesticks
     *
     * @return candle sticks of a the day
     */
    const candles& get_candles() const noexcept;


    /**
     * @brief the duration of the candles in the stock data
     *
     * @return duration in seconds
     */
    std::int32_t candle_size() const noexcept;

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
    std::string columns_str( size_t i_index ) const noexcept;
};


struct csv_data
{
public:
    fs::path m_filepath{};  // Name of file from which csv data has been loaded

    std::list<date_s> dates{};  // List of unique dates

    std::unordered_map<date_s, stock_data> stock_map{};  // Map of date to stock data
};
