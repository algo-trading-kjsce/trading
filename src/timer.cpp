/**
 * @file timer.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "timer.hpp"


timer::timer() noexcept
{
    m_points.push_back(std::chrono::system_clock::now());
}


std::chrono::milliseconds timer::total_time() noexcept
{
    auto time{ std::chrono::system_clock::now() };

    auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(time - m_points.front()) };

    m_points.push_back(time);

    return duration;
}


std::chrono::milliseconds timer::lap_time() noexcept
{
    auto time{ std::chrono::system_clock::now() };

    auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>(time - m_points.back()) };

    m_points.push_back(time);

    return duration;
}
