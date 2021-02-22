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

#include <list>
#include <chrono>

class timer
{
private:
    std::list<std::chrono::steady_clock::time_point> m_points{};

public:
    /**
     * @brief Construct a new timer object
     *
     */
    explicit timer() noexcept
    {
        m_points.push_back( std::chrono::steady_clock::now() );
    }


    /**
     * @brief Return duration since object was created
     *
     * @return time in milliseconds
     */
    std::chrono::milliseconds total_time() noexcept
    {
        auto time{ std::chrono::steady_clock::now() };

        auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>( time - m_points.front() ) };

        m_points.push_back( std::move( time ) );

        return duration;
    }


    /**
     * @brief Return duration since last recorded time
     *
     * @return time in milliseconds
     */
    std::chrono::milliseconds lap_time() noexcept
    {
        auto time{ std::chrono::steady_clock::now() };

        auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>( time - m_points.back() ) };

        m_points.push_back( std::move( time ) );

        return duration;
    }
};
