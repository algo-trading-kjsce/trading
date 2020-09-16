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

#include "csv_data.hpp"

namespace ta_utilities
{

/**
 * @brief find patterns in the stock price ranges using TA-Lib
 * 
 * @param io_csv_data csv data
 * 
 * @return number of times each strategy was triggered
 */
std::vector<int> find_patterns(csv_data_s& io_csv_data);

}