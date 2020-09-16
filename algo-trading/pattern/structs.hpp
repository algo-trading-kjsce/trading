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

#include <list>
#include <vector>
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
    std::string to_str() const noexcept;

    /**
     * @brief Equality operator for date objects
     *
     * @param i_lhs first date object
     * @param i_rhs second date object
     * @return true if dates are equal
     */
    friend bool operator==(const date_s& i_lhs, const date_s& i_rhs) noexcept;
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
    std::string to_str() const noexcept;
};


/**
 * @brief Struct to hold candle stick information
 *
 */
struct candle_s
{
    int index{};    // Index in the csv file

    date_s date{};    // date of the candle
    time_s time{};    // time of the candle

    double open{};    // open price of candle
    double high{};    // high price of candle
    double low{};     // low price of candle
    double close{};   // close price of can

    int volume{};    // number of shares available
};

/**
 * @brief Write candle object to stream
 * 
 * @param io_stream stream to write to
 * @param i_candle object to be written out
 * @return std::ostream
 */
std::ostream& operator<<(std::ostream& io_stream, const candle_s& i_candle);


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


using csv_result_t = std::list<std::pair<std::string, std::vector<int>>>;
