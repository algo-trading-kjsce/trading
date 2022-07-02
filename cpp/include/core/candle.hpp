/**
 * @file candle.hpp
 * @author ashwinn76
 * @brief candle struct
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "date.hpp"
#include "time.hpp"

#include <cstdint>
#include <optional>
#include <sstream>
#include <string>

/**
 * @brief whether the candle is an increasing or decreasing one
 * 
 */
enum class candle_direction
{
    none,
    up,
    down,
    stable
};


/**
 * @brief Struct to hold candle stick information
 *
 */
struct candle_s
{
    std::int32_t index{};  // Index in the csv file
    std::int32_t volume{};  // number of shares available

    date_s date{};  // date of the candle
    time_s time{};  // time of the candle

    double open{};  // open price of candle
    double high{};  // high price of candle
    double low{};  // low price of candle
    double close{};  // close price of candle

    candle_direction direction() const noexcept;

    /**
     * @brief Write candle object to stream
     *
     * @param io_stream stream to write to
     */
    void write_csv_text( std::ostream& io_stream ) const;

    /**
     * @brief Equality operator overload for candle objects
     *
     * @param i_lhs First candle object
     * @param i_rhs Second candle object
     * @return true if candles are equal
     */
    constexpr friend auto operator==( const candle_s& i_lhs, const candle_s& i_rhs ) noexcept
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
     * @brief Get the candle object from the line
     *
     * @param i_line string to be parsed
     * @return candle parsed from string
     */
    std::optional<candle_s> try_create_candle( const std::string& i_line );
