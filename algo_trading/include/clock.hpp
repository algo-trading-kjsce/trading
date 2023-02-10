/**
 * @file clock.hpp
 * @author ashwinn76
 * @brief Definition of clock
 * @version 0.1
 * @date 2022-07-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <chrono>

namespace trading
{
using clock = std::chrono::steady_clock;
using time_point = typename clock::time_point;
using duration = typename clock::duration;
}  // namespace trading
