/**
 * @file ta_utils.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once
#include <vector>

#include "../common/csv_data.hpp"

namespace trading::ta_utilities
{
/**
 * @brief find patterns in the stock price ranges using TA-Lib
 *
 * @param io_csv_data csv data
 *
 * @return number of times each strategy was triggered
 */
std::vector<std::int32_t> find_patterns( csv_data& io_csv_data );

}