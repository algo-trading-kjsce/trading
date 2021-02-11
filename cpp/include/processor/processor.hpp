/**
 * @file processor.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cstdint>

#include "enums.hpp"

namespace trading
{
/**
 * @brief Change resolution of input csv files
 *
 * @param i_argc number of arguments
 * @param i_argv arguments in const char* format, can be csv files or directories containing csv files
 * @param i_multi_threaded whether to process asynchronously
 * @return result of resolution change processing
 */
trading_app_result change_resolution( int i_argc, const char* i_argv[], bool i_multi_threaded );


/**
 * @brief Identify patterns in the arg csv files passed in
 *
 * @param i_argc number of arguments
 * @param i_argv arguments in const char* format, can be csv files or directories containing csv files
 * @param i_multi_threaded whether to process asynchronously
 * @return result of pattern processing
 */
trading_app_result identify_patterns( int i_argc, const char* i_argv[], bool i_multi_threaded );
}