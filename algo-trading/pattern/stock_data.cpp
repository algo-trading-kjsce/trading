/**
 * @file stock_data.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <sstream>
#include <algorithm>

#include "../include/includes.hpp"
#include "stock_data.hpp"


stock_data::stock_data(date_s date) noexcept : m_date{ std::move(date) }
{
}


void stock_data::add_candle(candle_s i_candle) noexcept
{
    m_candles.push_back(std::move(i_candle));
}


void stock_data::add_custom_column(std::vector<double> i_custom_column) noexcept
{
    m_custom_columns.push_back(std::move(i_custom_column));
}


void stock_data::add_strategy_column(std::vector<int> i_strategy_column) noexcept
{
    m_strategy_columns.push_back(std::move(i_strategy_column));
}


stock_data::stock_iterator stock_data::begin() const noexcept
{
    return m_candles.begin();
}


stock_data::stock_iterator stock_data::end() const noexcept
{
    return m_candles.end();
}


raw_stock_input_s stock_data::raw_values() const noexcept
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


std::string stock_data::columns_str(size_t i_index) const noexcept
{
    auto ss{ std::stringstream{} };

    for (auto&& column : m_custom_columns)
    {
        ss << std::defaultfloat << column.at(i_index) << delimiter;
    }

    for (auto&& column : m_strategy_columns)
    {
        ss << std::defaultfloat << column.at(i_index) << delimiter;
    }

    return ss.str();
}
