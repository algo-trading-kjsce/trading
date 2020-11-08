/**
 * @file resolution.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <iostream>
#include <algorithm>

#include "timer.hpp"
#include "type_trait_utils.hpp"

#include "resolution.hpp"

#include "utilities.hpp"

namespace trading::csv::resolution
{

std::int32_t find_candle_size(const csv_data& i_csv_data) noexcept
{
    return i_csv_data.stock_map.begin()->second.candle_size();
}


csv_data change_resolution(const csv_data& i_csv_data, std::int32_t i_new_candle_size)
{
    auto tmr{ timer{} };

    auto new_csv_data{ csv_data{} };

    auto old_candle_size{ find_candle_size(i_csv_data) };

    if (i_new_candle_size % old_candle_size != 0 && ((i_new_candle_size / old_candle_size) > 1))
    {
        throw std::invalid_argument{ "New resolution is not valid" };
    }

    auto resolution_scale{ i_new_candle_size / old_candle_size };

    auto new_index{ 0_i32 };

    for (auto&& date : i_csv_data.dates)
    {
        new_csv_data.dates.push_back(date);

        auto [new_stock_data_iter, _] = new_csv_data.stock_map.try_emplace(date, stock_data{ i_new_candle_size, date });

        auto& old_stock_data{ i_csv_data.stock_map.at(date) };

        auto iter{ old_stock_data.begin() };

        auto remaining_candles{ static_cast<std::int32_t>(old_stock_data.candles().size()) };

        while (iter != old_stock_data.end())
        {
            auto max_possible_candles{ std::min(resolution_scale, remaining_candles) };

            auto end_iter{ std::next(iter, max_possible_candles) };

            auto new_candle{ trading::csv::resolution::combine_candles(date, iter, end_iter, max_possible_candles) };

            new_candle.index = new_index++;

            new_stock_data_iter->second.add_candle(std::move(new_candle));

            iter = end_iter;

            remaining_candles -= max_possible_candles;
        }
    }

    {
        auto m{ trading::utilities::io_lock{} };
        std::cout << "Resolution changed from " << old_candle_size << " to " << i_new_candle_size << " in " << tmr.total_time().count() << "ms" << std::endl;
    }

    return new_csv_data;
}

}