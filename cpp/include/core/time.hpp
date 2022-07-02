/**
 * @file time.hpp
 * @author ashwinn76
 * @brief time struct
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>

/**
 * @brief Struct to hold time information
 *
 */
struct time_s
{
    std::int32_t hours{};  // hours
    std::int32_t minutes{};  // minutes
    std::int32_t seconds{};  // seconds

    /**
     * @brief Convert to string format
     *
     * @return string form of time
     */
    std::string to_string() const;

    void add_minutes( std::int32_t i_minutes ) noexcept;

    /**
     * @brief Equality operator for time objects
     *
     * @param i_lhs first time object
     * @param i_rhs second time object
     * @return true if times are equal
     */
    constexpr friend auto operator==( const time_s& i_lhs, const time_s& i_rhs ) noexcept
    {
        return i_lhs.hours == i_rhs.hours && i_lhs.minutes == i_rhs.minutes && i_lhs.seconds == i_rhs.seconds;
    }

    /**
     * @brief Inequality operator for time objects
     *
     * @param i_lhs first time object
     * @param i_rhs second time object
     * @return true if objects are not equal
     */
    constexpr friend auto operator!=( const time_s& i_lhs, const time_s& i_rhs ) noexcept
    {
        return !( i_lhs == i_rhs );
    }

    /**
     * @brief subtraction operator overload for time objects
     *
     * @param i_lhs first time object
     * @param i_rhs second time object
     * @return difference in seconds
     */
    constexpr friend auto operator-( const time_s& i_lhs, const time_s& i_rhs ) noexcept
    {
        auto convert_to_seconds = []( auto&& i_time ) {
            return ( ( i_time.hours * 3600 ) + ( i_time.minutes * 60 ) + i_time.seconds );
        };

        return convert_to_seconds( i_lhs ) - convert_to_seconds( i_rhs );
    }
};

    /**
     * @brief create date and time from string
     *
     * @param i_string input string to be parsed
     * @return date and time
     */
    std::optional<time_s> try_create_time( const std::string& i_string );
