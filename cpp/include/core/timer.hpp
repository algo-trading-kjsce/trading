/**
 * @file timer.hpp
 * @author ashwinn76
 * @brief timer class to record performance
 * @version 0.1
 * @date 2020-09-11
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <chrono>
#include <list>

class timer
{
private:
    std::list<std::chrono::steady_clock::time_point> m_points{};

public:
    /**
     * @brief Construct a new timer object
     *
     */
    explicit timer() noexcept;

    /**
     * @brief Return duration since object was created
     *
     * @return time in milliseconds
     */
    std::chrono::milliseconds total_time() noexcept;

    /**
     * @brief Return duration since last recorded time
     *
     * @return time in milliseconds
     */
    std::chrono::milliseconds lap_time() noexcept;
};
