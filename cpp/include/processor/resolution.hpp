/**
 * @file resolution.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <list>
#include <type_traits>

#include "../common/csv_data.hpp"

namespace trading::csv::resolution
{

/**
 * @brief Find the current resolution of the csv file
 *
 * @param i_csv_data incoming csv data
 * @return duration for which candles are recorded in the csv file
 */
std::int32_t find_candle_size(const csv_data& i_csv_data) noexcept;


/**
 * @brief combine a series of candles to make a single candle from [i_start, i_end)
 * 
 * @tparam _const_iterator constant iterator to a container of candles
 * @param i_date date of the candles
 * @param i_start start of candle container
 * @param i_end end of candle container
 * @param i_count number of candles
 * @return combined single candle
 */
template <typename _const_iterator>
auto combine_candles(date_s i_date, _const_iterator i_start, _const_iterator i_end, std::size_t i_count) noexcept
{
    static_assert(std::is_convertible_v<decltype(std::declval<_const_iterator>().operator*()), candle_s>, "This is not a list of candles!");

    auto new_candle{ candle_s{} };

    new_candle.date = std::move(i_date);
    new_candle.time = i_start->time;
    new_candle.open = i_start->open;
    new_candle.low = i_start->low;
    new_candle.close = std::prev(i_end)->close;

    for (auto iter{ i_start }; iter != i_end; ++iter)
    {
        new_candle.volume += iter->volume;

        new_candle.low = std::min(iter->low, new_candle.low);

        new_candle.high = std::max(iter->high, new_candle.high);
    }

    new_candle.volume = static_cast<std::int32_t>(new_candle.volume / i_count);

    return new_candle;
}


/**
 * @brief change the resolution of the csv file to the input candle stick size
 *
 * @param i_csv_data old csv data
 * @param i_new_candle_size new size of the candle stick
 *
 * @throw invalid_argument if new resolution is not completely divisible by old resolution
 *
 * @return updated csv data
 */
csv_data change_resolution(const csv_data& i_csv_data, std::int32_t i_new_candle_size);

}