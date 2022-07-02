/**
 * @file date.hpp
 * @author ashwinn76
 * @brief date struct
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
 * @brief Enumeration for months in a year
 *
 */
enum class month
{
    jan = 1,
    feb = 2,
    mar = 3,
    apr = 4,
    may = 5,
    jun = 6,
    jul = 7,
    aug = 8,
    sep = 9,
    oct = 10,
    nov = 11,
    dec = 12,
};


/**
 * @brief Struct to hold date information
 *
 */
struct date_s
{
    // year
    std::int32_t m_year{};

    // month in the year
    month m_month{};

    // day of the month
    std::int32_t m_day{};

    /**
     * @brief Convert to string format
     *
     * @return string form of date
     */
    std::string to_string() const;

    /**
     * @brief Equality operator for date objects
     *
     * @param i_lhs first date object
     * @param i_rhs second date object
     * @return true if dates are equal
     */
    constexpr friend auto operator==( const date_s& i_lhs, const date_s& i_rhs ) noexcept
    {
        return i_lhs.m_year == i_rhs.m_year && i_lhs.m_month == i_rhs.m_month && i_lhs.m_day == i_rhs.m_day;
    }

    /**
     * @brief Inequality operator for date objects
     *
     * @param i_lhs first date object
     * @param i_rhs second date object
     * @return true if objects are not equal
     */
    constexpr friend auto operator!=( const date_s& i_lhs, const date_s& i_rhs ) noexcept
    {
        return !( i_lhs == i_rhs );
    }
};

    /**
     * @brief Create date object if possible
     *
     * @param i_string Input string for parsing
     * @return optional date object
     */
    std::optional<date_s> try_create_date( const std::string& i_string );

namespace std
{
/**
 * @brief Template specialization for date_s
 *
 * @tparam date_s template specialization
 */
template<>
struct hash<date_s>
{
    /**
     * @brief Operator overload for finding lesser of two objects
     *
     * @param i_date Date object to hash
     *
     * @return hash value of date object
     */
    constexpr auto operator()( const date_s& i_date ) const noexcept
    {
        return ( ( i_date.m_year * 10000 ) + ( static_cast<std::int32_t>( i_date.m_month ) * 100 ) + i_date.m_day );
    }
};

}
