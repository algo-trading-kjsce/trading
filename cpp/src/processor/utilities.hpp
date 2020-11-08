/**
 * @file utilities.hpp
 * @author ashwinn76
 * @brief Utility functions
 * @version 0.1
 * @date 2020-09-11
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <list>
#include <string>
#include <vector>
#include <filesystem>
#include <mutex>

#include "csv_data.hpp"

using strategy_occurrence_count_t = std::list<std::pair<std::string, std::vector<std::int32_t>>>;

namespace trading::utilities
{

/**
 * @brief class to synchronously manage writing to std::cout
 * 
 */
class io_lock
{
public:
    io_lock();
    ~io_lock() = default;

private:
    std::lock_guard<std::mutex> m_lock;
};


/**
 * @brief finds all files in the directory, filters if extension supplied
 *
 * @param i_path path of directory
 * @param i_extension extension of files to be found
 * @return list of files found
 */
std::list<std::filesystem::path> find_files(const char* i_path, const char* i_extension = nullptr);


/**
 * @brief read in a csv file
 *
 * @param i_filepath path of the csv file
 * @return csv data
 */
csv_data read_initial_csv(const std::filesystem::path& i_filepath);


/**
 * @brief write out a csv file
 *
 * @param i_csv_data csv data to be written out
 * @param i_path path to written file
 * @param i_write_strategies flag to specify whether strategy information should be written out
 */
void write_csv(const csv_data& i_csv_data, const std::filesystem::path& i_path, bool i_write_strategies);


/**
 * @brief write results of strategy occurrence in stock information
 *
 * @param i_path path of file
 * @param i_csv_result results to be written out
 */
void write_strategy_occurrences(const std::filesystem::path& i_path, const strategy_occurrence_count_t& i_csv_result);

}
