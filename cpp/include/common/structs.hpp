/**
 * @file structs.hpp
 * @author ashwinn76
 * @brief Simple structs used in the source code
 * @version 0.1
 * @date 2020-09-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <vector>

/**
 * @brief Structure to contain raw input for TA-Lib
 *
 */
struct raw_stock_input_s
{
    std::vector<double> opens{};  // open prices in array form
    std::vector<double> highs{};  // high prices in array form
    std::vector<double> lows{};  // low prices in array form
    std::vector<double> closes{};  // close prices in array form
};
