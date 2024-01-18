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
#include <mutex>
#include <string>
#include <vector>

#include "../common/fs_include.hpp"
#include "../common/stock_data.hpp"
#include "../common/type_trait_utils.hpp"

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
 * @brief Output to terminal asynchronously
 *
 * @tparam _Args Types of the rest of the arguments
 * @param i_args Arguments
 */
template<typename... _Args>
void async_cout( _Args&&... i_args )
{
    static_assert( sizeof...( i_args ) > 0_sz, "Empty argument list not allowed!" );

    auto lk{ io_lock{} };

    ( std::cout << ... << std::forward<_Args>( i_args ) );
}


/**
 * @brief finds all files in the directory, filters if extension supplied
 *
 * @param i_path path of directory
 * @param i_extension extension of files to be found
 * @return list of files found
 */
std::list<fs::path> find_files( const char* i_path, const char* i_extension = nullptr );


/**
 * @brief read in a csv file
 *
 * @param i_filepath path of the csv file
 * @return csv data
 */
csv_data read_initial_csv( const fs::path& i_filepath );


/**
 * @brief write out a csv file
 *
 * @param i_csv_data csv data to be written out
 * @param i_path path to written file
 * @param i_write_strategies flag to specify whether strategy information should be written out
 */
void write_csv( const csv_data& i_csv_data, const fs::path& i_path, bool i_write_strategies );


/**
 * @brief write results of strategy occurrence in stock information
 *
 * @param i_path path of file
 * @param i_csv_result results to be written out
 */
void write_strategy_occurrences( const fs::path& i_path, const strategy_occurrence_count_t& i_csv_result );

}
