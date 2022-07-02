/**
 * @file timer.cpp
 * @author ashwinn76
 * @brief definition of timer member functions
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "timer.hpp"

    timer::timer() noexcept
    {
        m_points.push_back( std::chrono::steady_clock::now() );
    }

    std::chrono::milliseconds timer::total_time() noexcept
    {
        auto time{ std::chrono::steady_clock::now() };

        auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>( time - m_points.front() ) };

        m_points.push_back( std::move( time ) );

        return duration;
    }

    std::chrono::milliseconds timer::lap_time() noexcept
    {
        auto time{ std::chrono::steady_clock::now() };

        auto duration{ std::chrono::duration_cast<std::chrono::milliseconds>( time - m_points.back() ) };

        m_points.push_back( std::move( time ) );

        return duration;
    }
