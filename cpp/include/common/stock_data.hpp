/**
 * @file stock_data.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "fs_include.hpp"
#include "structs.hpp"

/**
 * @brief A collection of daily stock fluctuations
 *
 */
class stock_data
{
private:
    date_s m_date{};  // date of the stock data

    std::int32_t m_candle_size{};  // candle size in minutes

    std::list<candle_s> m_candles{};  // List of candles of that particular stock data

    // List of probable strategies identified by TA-Lib
    std::unordered_map<ta_helper::ta_strategy, std::vector<std::int32_t>> m_strategies{};

public:
    using iterator = typename decltype( m_candles )::iterator;
    using const_iterator = typename decltype( m_candles )::const_iterator;

    /**
     * @brief Construct a new CStockData object
     *
     * @param i_candle_size Size of candle
     * @param i_date Date of stock information
     */
    explicit stock_data( std::int32_t i_candle_size, date_s i_date ) noexcept :
        m_date{ std::move( i_date ) }, m_candle_size{ i_candle_size }
    {
    }


    /**
     * @brief Add new candle stick to stock information
     *
     * @param i_candleStick candle stick to add
     *
     * @return true if candle was added
     */
    bool add_candle( candle_s i_candle ) noexcept
    {
        if( !m_candles.empty() )
        {
            auto new_candle_size{ i_candle.time - m_candles.back().time };

            if( new_candle_size > m_candle_size )
            {
                auto number_of_extra_candles{ ( new_candle_size / m_candle_size ) - 1 };

                auto ref_candle{ m_candles.back() };

                for( auto i{ 1 }; i <= number_of_extra_candles; ++i )
                {
                    auto new_time{ ref_candle.time };

                    new_time.add_minutes( m_candle_size * i );

                    m_candles.push_back( candle_s{ -1,
                                                   0,
                                                   ref_candle.date,
                                                   std::move( new_time ),
                                                   ref_candle.close,
                                                   ref_candle.close,
                                                   ref_candle.close,
                                                   ref_candle.close } );
                }
            }
            else if( new_candle_size < m_candle_size )
            {
                return false;
            }
        }

        m_candles.push_back( std::move( i_candle ) );

        return true;
    }


    /**
     * @brief Add new strategy pattern to the stock data
     *
     * @param i_strategy type of strategy
     * @param i_custom_column custom value to add
     */
    void add_strategy_column( ta_helper::ta_strategy i_strategy, std::vector<std::int32_t> i_strategy_column ) noexcept
    {
        m_strategies.try_emplace( i_strategy, i_strategy_column );
    }


    /**
     * @brief begin iterator
     *
     * @return begin iterator for stock_data object
     */
    iterator begin() noexcept
    {
        return m_candles.begin();
    }


    /**
     * @brief end iterator
     *
     * @return beyond-the-end iterator for stock_data object
     */
    iterator end() noexcept
    {
        return m_candles.end();
    }


    /**
     * @brief const begin iterator
     *
     * @return begin iterator for stock_data object
     */
    const_iterator begin() const noexcept
    {
        return m_candles.begin();
    }


    /**
     * @brief const end iterator
     *
     * @return beyond-the-end iterator for stock_data object
     */
    const_iterator end() const noexcept
    {
        return m_candles.end();
    }


    /**
     * @brief get access to candlesticks
     *
     * @return candle sticks of a the day
     */
    decltype( auto ) candles() const noexcept
    {
        return ( m_candles );
    }


    /**
     * @brief the duration of the candles in the stock data
     *
     * @return duration in seconds
     */
    auto candle_size() const noexcept
    {
        return m_candle_size;
    }


    /**
     * @brief Get column-wise data for the day
     *
     * @return open, high, low and close values for the day
     */
    raw_stock_input_s raw_values() const noexcept
    {
        auto raw_input{ raw_stock_input_s{} };

        raw_input.opens.reserve( m_candles.size() );
        raw_input.highs.reserve( m_candles.size() );
        raw_input.lows.reserve( m_candles.size() );
        raw_input.closes.reserve( m_candles.size() );

        for( auto&& candle : m_candles )
        {
            raw_input.opens.push_back( candle.open );
            raw_input.highs.push_back( candle.high );
            raw_input.lows.push_back( candle.low );
            raw_input.closes.push_back( candle.close );
        }

        return raw_input;
    }


    /**
     * @brief Return the newly added column values in csv format
     *
     * @param i_index row index
     *
     * @return std::string
     */
    std::string columns_str( size_t i_index ) const noexcept
    {
        auto ss{ std::stringstream{} };

        for( auto strategy{ static_cast<ta_helper::ta_strategy>( 0 ) }; strategy < ta_helper::ta_strategy::__LAST;
             strategy = static_cast<ta_helper::ta_strategy>( strategy + 1 ) )
        {
            ss << std::defaultfloat << m_strategies.at( strategy ).at( i_index ) << csv_delimiter;
        }

        return ss.str();
    }
};


struct csv_data
{
public:
    fs::path m_filepath{};  // Name of file from which csv data has been loaded

    std::list<date_s> dates{};  // List of unique dates

    std::unordered_map<date_s, stock_data> stock_map{};  // Map of date to stock data
};
