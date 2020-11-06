/**
 * @file utilities.cpp
 * @author ashwinn76
 * @brief Implementation of utility algorithms
 * @version 0.1
 * @date 2020-09-11
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <fstream>
#include <sstream>
#include <iostream>

#include <optional>
#include <filesystem>

#include "type_trait_utils.hpp"
#include "common_utilities.hpp"
#include "includes.hpp"
#include "timer.hpp"


#include "utilities.hpp"

namespace
{

auto get_candle_size(const std::filesystem::path& i_filepath)
{
    auto str{ std::string{} };

    if (auto file{ std::ifstream{i_filepath} }; file.good())
    {
        std::getline(file, str);

        std::getline(file, str);
        if (auto c1{ candle_s::try_create_candle(str) }; c1.has_value())
        {
            std::getline(file, str);
            if (auto c2{ candle_s::try_create_candle(str) }; c2.has_value())
            {
                return c2->time - c1->time;
            }
        }
    }

    return 0;
}

}


namespace trading::utilities
{

std::list<std::filesystem::path> find_files(const char* i_path, const char* i_extension/* = nullptr*/)
{
    auto files{ std::list<std::filesystem::path>{} };

    auto path{ std::filesystem::path{i_path} };

    if (std::filesystem::is_directory(path))
    {
        for (auto&& file : std::filesystem::directory_iterator(i_path))
        {
            auto& file_path{ file.path() };

            if (i_extension == nullptr || file_path.extension() == i_extension)
            {
                files.push_back(file_path);
            }
        }
    }
    else if (std::filesystem::is_regular_file(path))
    {
        if (i_extension == nullptr || path.extension() == i_extension)
        {
            files.push_back(path);
        }
    }

    return files;
}


csv_data read_initial_csv(const std::filesystem::path& i_filepath)
{
    if (!std::filesystem::exists(i_filepath))
    {
        throw std::filesystem::filesystem_error{ "Path not found", i_filepath, {} };
    }

    auto tmr{ timer{} };

    auto stockInformation{ std::unordered_map<date_s, stock_data>{} };

    auto dates{ std::list<date_s>{} };

    auto column_names_str{ std::string{} };

    auto candle_size{ get_candle_size(i_filepath) };

    if (auto file{ std::ifstream{i_filepath} }; file.good())
    {
        std::getline(file, column_names_str);

        while (file.good() && !file.eof())
        {
            auto line{ std::string{} };

            std::getline(file, line);

            if (auto o_candle{ candle_s::try_create_candle(line) }; o_candle.has_value())
            {
                auto date{ o_candle->date };

                auto [iter, newly_inserted] = stockInformation.try_emplace(date_s{ date }, stock_data{ candle_size, date });

                iter->second.add_candle(std::move(o_candle.value()));

                if (newly_inserted)
                {
                    dates.push_back(date);
                }
            }
        }
    }

    std::cout << "File " << i_filepath.filename().string() << " read in " << tmr.total_time().count() << "ms" << std::endl;

    return { i_filepath, std::move(dates), std::move(stockInformation) };
}


void write_csv(const csv_data& i_csv_data, const std::filesystem::path& i_path, bool i_write_strategies)
{
    auto tmr{ timer{} };

    if (auto result_directory{ i_path.parent_path() }; !std::filesystem::exists(result_directory))
    {
        std::filesystem::create_directories(result_directory);
    }

    if (auto file{ std::ofstream{i_path} }; file.good())
    {
        if (i_write_strategies)
        {
            file << common::utilities::all_columns_str << std::endl;
        }
        else
        {
            file << common::utilities::basic_columns_str << std::endl;
        }


        for (auto&& date : i_csv_data.dates)
        {
            auto idx{ 0_sz };

            auto date_str{ date.to_str() };

            auto& day_data{ i_csv_data.stock_map.at(date) };

            for (auto&& candle : day_data)
            {
                candle.write_csv_text(file);

                if (i_write_strategies)
                {
                    file << delimiter << day_data.columns_str(idx++) << std::endl;
                }
                else
                {
                    file << std::endl;
                }

            }
        }
    }

    std::cout << "File " << i_path.filename().string() << " written in " << tmr.total_time().count() << "ms" << std::endl;
}


void write_strategy_occurrences(
    const std::filesystem::path& i_path,
    const strategy_occurrence_count_t& i_csv_result)
{
    if (auto file{ std::ofstream{i_path} }; file.good())
    {
        file << "Name" << delimiter << common::utilities::strategy_columns_str << "Total" << std::endl;

        auto total_occurrences_per_strategy{ std::vector<std::int32_t>(54, 0) };

        for (auto&& [stock_name, strategy_occurrences] : i_csv_result)
        {
            auto total_occurrences_for_stock{ 0 };

            file << stock_name << delimiter;

            auto idx{ 0_sz };

            for (auto&& occurrence : strategy_occurrences)
            {
                total_occurrences_per_strategy.at(idx++) += occurrence;

                total_occurrences_for_stock += occurrence;

                file << occurrence << delimiter;
            }

            file << total_occurrences_for_stock << std::endl;
        }

        file << "Total" << delimiter;

        for (auto&& occurrence : total_occurrences_per_strategy)
        {
            file << occurrence << delimiter;
        }
    }
}

}
