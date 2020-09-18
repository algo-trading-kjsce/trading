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

#include "csv_data.hpp"

using strategy_occurrence_count_t = std::list<std::pair<std::string, std::vector<int>>>;

namespace utilities
{

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
 */
void write_csv_with_strategies(const csv_data& i_csv_data, const std::filesystem::path& i_path);


/**
 * @brief write results of strategy occurrence in stock information
 * 
 * @param i_path path of file
 * @param i_csv_result results to be written out
 * @param i_strategy_names names of the strategy in csv format
 */
void write_strategy_occurrences(
    const std::filesystem::path& i_path,
    const strategy_occurrence_count_t& i_csv_result,
    const std::string& i_strategy_names);

}
