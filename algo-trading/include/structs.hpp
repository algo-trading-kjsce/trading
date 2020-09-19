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
    int m_year{};

    // month in the year
    month m_month{};

    // day of the month
    int m_day{};

    /**
     * @brief Convert to string format
     *
     * @return string form of date
     */
    std::string to_str() const noexcept
    {
        auto ss{ std::stringstream{} };

        ss <<
            std::setw(2) << std::setfill('0') << m_year << '-' <<
            std::setw(2) << std::setfill('0') << static_cast<int>(m_month) << '-' <<
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
    friend bool operator==(const date_s& i_lhs, const date_s& i_rhs) noexcept
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
        return ((i_date.m_year * 10000) + (static_cast<int>(i_date.m_month) * 100) + i_date.m_day);
    }
};

}


/**
 * @brief Struct to hold time information
 *
 */
struct time_s
{
    int hours{};      // hours
    int minutes{};    // minutes
    int seconds{};    // seconds


    /**
     * @brief Convert to string format
     *
     * @return string form of time
     */
    std::string to_str() const noexcept
    {
        auto ss{ std::stringstream{} };

        ss <<
            std::setw(2) << std::setfill('0') << hours << ':' <<
            std::setw(2) << std::setfill('0') << minutes << ':' <<
            std::setw(2) << std::setfill('0') << seconds;

        return ss.str();
    }
};


/**
 * @brief Struct to hold candle stick information
 *
 */
struct candle_s
{
    int index{};    // Index in the csv file
    int volume{};   // number of shares available

    date_s date{};    // date of the candle
    time_s time{};    // time of the candle

    double open{};    // open price of candle
    double high{};    // high price of candle
    double low{};     // low price of candle
    double close{};   // close price of can


/**
 * @brief Write candle object to stream
 * 
 * @param io_stream stream to write to
 * @param i_candle object to be written out
 * @return std::ostream
 */
friend std::ostream& operator<<(std::ostream& io_stream, const candle_s& i_candle)
{
    io_stream
        << i_candle.index << delimiter << i_candle.date.to_str() << ' ' << i_candle.time.to_str()
        << delimiter << i_candle.open << delimiter << i_candle.high << delimiter << i_candle.low << delimiter << i_candle.close
        << delimiter << i_candle.volume;

    return io_stream;
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
