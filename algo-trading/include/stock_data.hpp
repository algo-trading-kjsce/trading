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
#include <vector>
#include <unordered_map>

#include <sstream>
#include <algorithm>

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

    std::unordered_map<ta_helper::ta_strategy, std::vector<int>> m_strategies{};

public:
    using stock_iterator = typename decltype(m_candles)::const_iterator;

    /**
     * @brief Construct a new CStockData object
     *
     * @param date Date of stock information
     */
    explicit stock_data(date_s date) noexcept
        : m_date{ std::move(date) }
    {
    }


    /**
     * @brief Add new candle stick to stock information
     *
     * @param i_candleStick candle stick to add
     */
    void add_candle(candle_s i_candle) noexcept
    {
        m_candles.push_back(std::move(i_candle));
    }


    /**
     * @brief Add new strategy pattern to the stock data
     *
     * @param i_strategy type of strategy
     * @param i_custom_column custom value to add
     */
    void add_strategy_column(ta_helper::ta_strategy i_strategy, std::vector<int> i_strategy_column) noexcept
    {
        m_strategies.try_emplace(i_strategy, i_strategy_column);
    }


    /**
     * @brief begin iterator
     *
     * @return begin iterator for CStockData object
     */
    stock_iterator begin() const noexcept
    {
        return m_candles.begin();
    }


    /**
     * @brief end iterator
     *
     * @return beyond-the-end iterator for CStockData object
     */
    stock_iterator end() const noexcept
    {
        return m_candles.end();
    }


    /**
     * @brief Get column-wise data for the day
     *
     * @return open, high, low and close values for the day
     */
    raw_stock_input_s raw_values() const noexcept
    {
        auto raw_input{ raw_stock_input_s{} };

        raw_input.opens.reserve(m_candles.size());
        std::transform(m_candles.cbegin(), m_candles.cend(), std::back_inserter(raw_input.opens), [](auto&& candle)
        {
            return candle.open;
        });

        raw_input.highs.reserve(m_candles.size());
        std::transform(m_candles.cbegin(), m_candles.cend(), std::back_inserter(raw_input.highs), [](auto&& candle)
        {
            return candle.high;
        });

        raw_input.lows.reserve(m_candles.size());
        std::transform(m_candles.cbegin(), m_candles.cend(), std::back_inserter(raw_input.lows), [](auto&& candle)
        {
            return candle.low;
        });

        raw_input.closes.reserve(m_candles.size());
        std::transform(m_candles.cbegin(), m_candles.cend(), std::back_inserter(raw_input.closes), [](auto&& candle)
        {
            return candle.close;
        });

        return raw_input;
    }


    /**
     * @brief Return the newly added column values in csv format
     *
     * @param i_index row index
     *
     * @return std::string
     */
    std::string columns_str(size_t i_index) const noexcept
    {
        auto ss{ std::stringstream{} };

        for (auto strategy{ static_cast<ta_helper::ta_strategy>(0) }; strategy < ta_helper::ta_strategy::__LAST; strategy = static_cast<ta_helper::ta_strategy>(strategy + 1))
        {
            ss << std::defaultfloat << m_strategies.at(strategy).at(i_index) << delimiter;
        }

        return ss.str();
    }
};
