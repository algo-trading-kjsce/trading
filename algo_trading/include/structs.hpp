/**
 * @file structs.hpp
 * @author ashwinn76
 * @brief Declaration of POD types for project
 * @version 0.1
 * @date 2022-08-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <vector>

namespace trading
{
/**
 * @brief Price points converted into simple data structure
 *
 */
struct price_points
{
    std::vector<double> open;
    std::vector<double> high;
    std::vector<double> low;
    std::vector<double> close;
};
}  // namespace trading
