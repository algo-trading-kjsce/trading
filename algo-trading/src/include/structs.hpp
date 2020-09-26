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
#include <string>

#include <sstream>
#include <iomanip>

#include "includes.hpp"
#include "enums.hpp"


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
    auto to_str() const noexcept
    {
        auto ss{ std::stringstream{} };

        ss <<
            std::setw(2) << std::setfill('0') << m_year << '-' <<
            std::setw(2) << std::setfill('0') << static_cast<std::int32_t>(m_month) << '-' <<
            std::setw(2) << std::setfill('0') << m_day;

        return ss.str();
    }

    /**
     * @brief Equality operator for date objects
     *
     * @param i_lhs first date object
     * @param i_rhs second date object
     * @return true if dates are equal
     */
    friend auto operator==(const date_s& i_lhs, const date_s& i_rhs) noexcept
    {
        return i_lhs.m_year == i_rhs.m_year &&
            i_lhs.m_month == i_rhs.m_month &&
            i_lhs.m_day == i_rhs.m_day;
    }
};


namespace std
{

/**
 * @brief Template specialization for date_s
 *
 * @tparam date_s template specialization
 */
template <>
struct hash<date_s>
{
    /**
     * @brief Operator overload for finding lesser of two objects
     *
     * @param i_date Date object to hash
     *
     * @return hash value of date object
     */
    auto operator()(const date_s& i_date) const noexcept
    {
        return ((i_date.m_year * 10000) + (static_cast<std::int32_t>(i_date.m_month) * 100) + i_date.m_day);
    }
};

}


/**
 * @brief Struct to hold time information
 *
 */
struct time_s
{
    std::int32_t hours{};      // hours
    std::int32_t minutes{};    // minutes
    std::int32_t seconds{};    // seconds

    /**
     * @brief Convert to string format
     *
     * @return string form of time
     */
    auto to_str() const noexcept
    {
        auto ss{ std::stringstream{} };

        ss <<
            std::setw(2) << std::setfill('0') << hours << ':' <<
            std::setw(2) << std::setfill('0') << minutes << ':' <<
            std::setw(2) << std::setfill('0') << seconds;

        return ss.str();
    }


    void add_minutes(std::int32_t i_minutes) noexcept
    {
        minutes += i_minutes;

        if (minutes >= 60)
        {
            hours += minutes / 60;
            minutes %= 60;
        }
    }


    /**
     * @brief Equality operator for time objects
     *
     * @param i_lhs first time object
     * @param i_rhs second time object
     * @return true if times are equal
     */
    friend auto operator==(const time_s& i_lhs, const time_s& i_rhs) noexcept
    {
        return i_lhs.hours == i_rhs.hours &&
            i_lhs.minutes == i_rhs.minutes &&
            i_lhs.seconds == i_rhs.seconds;
    }


    /**
     * @brief subtraction operator overload for time objects
     *
     * @param i_lhs first time object
     * @param i_rhs second time object
     * @return difference in minutes
     */
    friend auto operator-(const time_s& i_lhs, const time_s& i_rhs) noexcept
    {
        auto convert_to_seconds = [](auto&& i_time)
        {
            return ((i_time.hours * 3600) + (i_time.minutes * 60) + i_time.seconds);
        };

        return (convert_to_seconds(i_lhs) - convert_to_seconds(i_rhs)) / 60;
    }
};


/**
 * @brief Struct to hold candle stick information
 *
 */
struct candle_s
{
    std::int32_t index{};    // Index in the csv file
    std::int32_t volume{};   // number of shares available

    date_s date{};    // date of the candle
    time_s time{};    // time of the candle

    double open{};    // open price of candle
    double high{};    // high price of candle
    double low{};     // low price of candle
    double close{};   // close price of candle


    /**
     * @brief Write candle object to stream
     *
     * @param io_stream stream to write to
     */
    void write_csv_text(std::ostream& io_stream) const
    {
        io_stream
            << index << delimiter << date.to_str() << ' ' << time.to_str()
            << delimiter << open << delimiter << high << delimiter << low << delimiter << close
            << delimiter << volume;
    }


    /**
     * @brief Equality operator overload for candle objects
     *
     * @param i_lhs First candle object
     * @param i_rhs Second candle object
     * @return true if candles are equal
     */
    friend auto operator==(const candle_s& i_lhs, const candle_s& i_rhs) noexcept
    {
        auto sameIndex{ i_lhs.index == i_rhs.index };
        auto sameVolume{ i_lhs.volume == i_rhs.volume };

        auto sameDate{ i_lhs.date == i_rhs.date };
        auto sameTime{ i_lhs.time == i_rhs.time };

        auto sameOpen{ i_lhs.open == i_rhs.open };
        auto sameHigh{ i_lhs.high == i_rhs.high };
        auto sameLow{ i_lhs.low == i_rhs.low };
        auto sameClose{ i_lhs.close == i_rhs.close };

        return sameIndex && sameVolume && sameDate && sameTime && sameOpen && sameHigh && sameLow && sameClose;
    }
};


/**
 * @brief Structure to contain raw input for TA-Lib
 *
 */
struct raw_stock_input_s
{
    std::vector<double> opens{};    // open prices in array form
    std::vector<double> highs{};    // high prices in array form
    std::vector<double> lows{};     // low prices in array form
    std::vector<double> closes{};   // close prices in array form
};