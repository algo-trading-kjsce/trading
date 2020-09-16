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

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <list>

#include "../include/includes.hpp"
#include "csv_data.hpp"


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
csv_data_s read_csv(const std::filesystem::path& i_filepath);


/**
 * @brief write out a csv file
 *
 * @param i_csv_data csv data to be written out
 * @param i_path path to written file
 */
void write_csv(const csv_data_s& i_csv_data, const std::filesystem::path& i_path);


/**
 * @brief write results of strategy occurrence in stock information
 * 
 * @param i_path path of file
 * @param i_csv_result results to be written out
 * @param i_strategy_names names of the strategy in csv format
 */
void write_results(const std::filesystem::path& i_path, const csv_result_t& i_csv_result, const std::string& i_strategy_names);

}
