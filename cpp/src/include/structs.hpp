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

#include <math.h>

#include <optional>

#include "includes.hpp"
#include "enums.hpp"

#include "type_trait_utils.hpp"

 /**
  * @brief Struct to hold date information
  *
  */
struct date_s
{
    // year
    std::uint16_t m_year{};

    // month in the year
    month m_month{};

    // day of the month
    day m_day{};

    /**
     * @brief Convert to string format
     *
     * @return string form of date
     */
    auto to_str() const noexcept
    {
        auto ss{ std::stringstream{} };

        ss <<
            std::setw(2) << std::setfill('0') << m_year << date_delimiter <<
            std::setw(2) << std::setfill('0') << +static_cast<std::uint8_t>(m_month) << date_delimiter <<
            std::setw(2) << std::setfill('0') << +static_cast<std::uint8_t>(m_day);

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


    /**
     * @brief Create date object if possible
     *
     * @param i_string Input string for parsing
     * @return optional date object
     */
    static auto try_create_date(const std::string& i_string)
    {
        auto o_date{ std::optional<date_s>{} };

        if (!i_string.empty())
        {
            o_date.emplace();

            auto ss{ std::stringstream{i_string} };

            auto parsed{ std::string{} };

            if (std::getline(ss, parsed, date_delimiter))
            {
                o_date->m_year = str_to<std::uint16_t>(parsed);
            }
            else
            {
                o_date.reset();
            }

            if (std::getline(ss, parsed, date_delimiter) && o_date.has_value())
            {
                o_date->m_month = str_to<month>(parsed);
            }
            else
            {
                o_date.reset();
            }

            if (std::getline(ss, parsed, date_delimiter) && o_date.has_value())
            {
                o_date->m_day = static_cast<day>(str_to<std::uint8_t>(parsed));
            }
            else
            {
                o_date.reset();
            }
        }

        return o_date;
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
        return
            (i_date.m_year * 10000) +
            (static_cast<std::int32_t>(i_date.m_month) * 100) +
            static_cast<std::int32_t>(i_date.m_day);
    }
};

}


/**
 * @brief Struct to hold time information
 *
 */
struct time_s
{
    std::uint8_t hours{};      // hours
    std::uint8_t minutes{};    // minutes
    std::uint8_t seconds{};    // seconds

    /**
     * @brief Convert to string format
     *
     * @return string form of time
     */
    auto to_str() const noexcept
    {
        auto ss{ std::stringstream{} };

        ss <<
            std::setw(2) << std::setfill('0') << +hours << time_delimiter <<
            std::setw(2) << std::setfill('0') << +minutes << time_delimiter <<
            std::setw(2) << std::setfill('0') << +seconds;

        return ss.str();
    }


    void add_minutes(std::uint8_t i_minutes) noexcept
    {
        constexpr auto mins_in_hour{ 60_ui8 };

        minutes += i_minutes;

        if (minutes >= mins_in_hour)
        {
            hours += minutes / mins_in_hour;
            minutes %= mins_in_hour;
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

    /**
     * @brief create date and time from string
     *
     * @param i_string input string to be parsed
     * @return date and time
     */
    static auto try_create_time(const std::string& i_string)
    {
        auto o_time{ std::optional<time_s>{} };

        if (!i_string.empty())
        {
            o_time.emplace();

            auto ss{ std::stringstream{i_string} };

            auto parsed{ std::string{} };

            if (std::getline(ss, parsed, time_delimiter))
            {
                o_time->hours = str_to<std::uint8_t>(parsed);
            }
            else
            {
                o_time.reset();
            }

            if (std::getline(ss, parsed, time_delimiter) && o_time.has_value())
            {
                o_time->minutes = str_to<std::uint8_t>(parsed);
            }

            if (std::getline(ss, parsed, time_delimiter) && o_time.has_value())
            {
                o_time->seconds = str_to<std::uint8_t>(parsed);
            }
        }

        return o_time;
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

    bool newly_added{ false };    // whether the candle was added for correction

    float open{};    // open price of candle
    float high{};    // high price of candle
    float low{};     // low price of candle
    float close{};   // close price of candle


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

        auto sameTempFlag{ i_lhs.newly_added == i_rhs.newly_added };
        
        auto sameOpen{ i_lhs.open == i_rhs.open };
        auto sameHigh{ i_lhs.high == i_rhs.high };
        auto sameLow{ i_lhs.low == i_rhs.low };
        auto sameClose{ i_lhs.close == i_rhs.close };

        return sameIndex && sameVolume && sameDate && sameTime && sameTempFlag && sameOpen && sameHigh && sameLow && sameClose;
    }


    /**
     * @brief Get the candle object from the line
     *
     * @param i_line string to be parsed
     * @return candle parsed from string
     */
    static auto try_create_candle(const std::string& i_line)
    {
        auto o_candle{ std::optional<candle_s>{} };

        if (!i_line.empty())
        {
            o_candle.emplace();

            auto stream{ std::stringstream{i_line} };

            auto parsed{ std::string{} };

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                o_candle->index = std::stoi(parsed);
            }
            else
            {
                o_candle.reset();
            }

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                auto ss{ std::stringstream{parsed} };

                if (std::getline(ss, parsed, ' '))
                {
                    if (auto o_date{ date_s::try_create_date(parsed) }; o_date.has_value())
                    {
                        o_candle->date = std::move(o_date.value());
                    }
                    else
                    {
                        o_candle.reset();
                    }
                }
                else
                {
                    o_candle.reset();
                }

                if (std::getline(ss, parsed, ' ') && o_candle.has_value())
                {
                    if (auto o_time{ time_s::try_create_time(parsed) }; o_time.has_value())
                    {
                        o_candle->time = std::move(o_time.value());
                    }
                    else
                    {
                        o_candle.reset();
                    }
                }
                else
                {
                    o_candle.reset();
                }
            }
            else
            {
                o_candle.reset();
            }

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                o_candle->open = floorf(std::stof(parsed) * 100.0f) / 100.0f;
            }
            else
            {
                o_candle.reset();
            }

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                o_candle->high = floorf(std::stof(parsed) * 100.0f) / 100.0f;
            }
            else
            {
                o_candle.reset();
            }

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                o_candle->low = floorf(std::stof(parsed) * 100.0f) / 100.0f;
            }
            else
            {
                o_candle.reset();
            }

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                o_candle->close = floorf(std::stof(parsed) * 100.0f) / 100.0f;
            }
            else
            {
                o_candle.reset();
            }

            if (std::getline(stream, parsed, delimiter) && o_candle.has_value())
            {
                o_candle->volume = std::stoi(parsed);
            }
            else
            {
                o_candle.reset();
            }
        }

        return o_candle;
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


/**
 * @brief Structure to hold information of a stock transaction
 * 
 */
struct transaction_data_s
{
    bool is_sale{}; // Whether the transaction was a buy or a sell

    time_s sale_time{}; // Time of sell/buy

    std::uint32_t n_shares{}; // Number of shares sold/bought

    double price{}; // Price at which shares were sold/bought
};

