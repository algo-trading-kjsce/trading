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

#include <filesystem>

#include "../include/includes.hpp"

#include "utilities.hpp"

#include "../include/timer.hpp"

namespace
{

/**
 * @brief create date and time from string
 *
 * @param i_str input string to be parsed
 * @return date and time
 */
auto str_to_time(const std::string& i_str)
{
    auto date{ date_s{} };
    auto time{ time_s{} };

    auto p{ static_cast<char*>(nullptr) };

    date.m_year = std::strtol(i_str.data(), std::addressof(p), 10);
    date.m_month = static_cast<month>(std::strtol(p + 1, std::addressof(p), 10));
    date.m_day = std::strtol(p + 1, std::addressof(p), 10);

    time.hours = std::strtol(p + 1, std::addressof(p), 10);
    time.minutes = std::strtol(p + 1, std::addressof(p), 10);
    time.seconds = std::strtol(p + 1, std::addressof(p), 10);

    return std::make_pair(date, time);
}


/**
 * @brief Get the candle object from the line
 *
 * @param i_line string to be parsed
 * @return candle parsed from string
 */
auto get_candle(const std::string& i_line)
{
    auto candle{ candle_s{} };

    auto stream{ std::stringstream{i_line} };

    auto parsed{ std::string{} };

    if (std::getline(stream, parsed, delimiter))
    {
        candle.index = std::stoi(parsed);
    }

    if (std::getline(stream, parsed, delimiter))
    {
        std::tie(candle.date, candle.time) = str_to_time(parsed);
    }

    if (std::getline(stream, parsed, delimiter))
    {
        candle.open = std::stod(parsed);
    }

    if (std::getline(stream, parsed, delimiter))
    {
        candle.high = std::stod(parsed);
    }

    if (std::getline(stream, parsed, delimiter))
    {
        candle.low = std::stod(parsed);
    }

    if (std::getline(stream, parsed, delimiter))
    {
        candle.close = std::stod(parsed);
    }

    if (std::getline(stream, parsed, delimiter))
    {
        candle.volume = std::stoi(parsed);
    }

    return candle;
}


/**
 * @brief Get the result path object
 *
 * @param i_filepath path of the old file
 * @return path of the file to be written out
 */
auto get_result_path(std::filesystem::path i_filepath)
{
    auto new_filepath{ i_filepath.parent_path() };
    new_filepath = new_filepath.parent_path();

    new_filepath.append("Results").append(i_filepath.filename().string());

    return new_filepath;
}

}


namespace utilities
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


csv_data_s read_csv(const std::filesystem::path& i_filepath)
{
    if (!std::filesystem::exists(i_filepath))
    {
        throw std::filesystem::filesystem_error{ "Path not found", i_filepath, {} };
    }

    auto tmr{ timer{} };

    auto stockInformation{ std::unordered_map<date_s, stock_data>{} };

    auto dates{ std::list<date_s>{} };

    auto column_names{ std::string{} };

    if (auto file{ std::ifstream{i_filepath} }; file.good())
    {
        std::getline(file, column_names);

        while (file.good() && !file.eof())
        {
            auto line{ std::string{} };

            std::getline(file, line);

            auto candle{ get_candle(line) };

            auto date{ candle.date };

            auto [iter, newly_inserted] = stockInformation.try_emplace(date_s{ date }, stock_data{ date });

            iter->second.add_candle(std::move(candle));

            if (newly_inserted)
            {
                dates.push_back(date);
            }
        }
    }

    std::cout << "File " << i_filepath.filename().string() << " read in " << tmr.total_time().count() << "ms\n";

    auto save_file_path{ get_result_path(i_filepath) };

    return { std::move(dates), std::move(stockInformation), std::move(column_names) };
}


void write_csv(const csv_data_s& i_csv_data, const std::filesystem::path& path)
{
    auto tmr{ timer{} };

    auto new_path{ get_result_path(path) };

    if (auto file{ std::ofstream{new_path} }; file.good())
    {
        file << i_csv_data.get_header() << '\n';

        for (auto&& date : i_csv_data.dates)
        {
            auto idx{ static_cast<size_t>(0) };

            auto date_str{ date.to_str() };

            auto& day_data{ i_csv_data.stock_map.at(date) };

            for (auto&& candle : day_data)
            {
                file << candle << delimiter << day_data.columns_str(idx) << '\n';

                ++idx;
            }
        }
    }

    std::cout << "File " << new_path.filename().string() << " written in " << tmr.total_time().count() << "ms\n";
}


void write_results(const std::filesystem::path& i_path, const csv_result_t& i_csv_result, const std::string& i_strategy_names)
{
    if (auto file{ std::ofstream{i_path} }; file.good())
    {
        file << "Name" << delimiter << i_strategy_names << "Total\n";

        auto total_occurrences_per_strategy{ std::vector<int>(54, 0) };

        for (auto&& [stock_name, strategy_occurrences] : i_csv_result)
        {
            auto total_occurrences_for_stock{ 0 };

            file << stock_name << delimiter;

            auto idx{ static_cast<size_t>(0) };

            for (auto&& occurrence : strategy_occurrences)
            {
                total_occurrences_per_strategy.at(idx++) += occurrence;

                total_occurrences_for_stock += occurrence;

                file << occurrence << delimiter;
            }

            file << total_occurrences_for_stock << '\n';
        }

        file << "Total" << delimiter;

        for (auto&& occurrence : total_occurrences_per_strategy)
        {
            file << occurrence << delimiter;
        }
    }
}

}
