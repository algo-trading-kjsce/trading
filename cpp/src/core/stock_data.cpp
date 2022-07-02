/**
 * @file stock_data.cpp
 * @author ashwinn76
 * @brief 
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common/includes.hpp"

#include "stock_data.hpp"

stock_data::stock_data( std::int32_t i_candle_size, date_s i_date ) noexcept :
        m_date{ std::move( i_date ) }, m_candle_size{ i_candle_size }
    {
    }

        bool stock_data::add_candle( candle_s i_candle ) noexcept
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

    void stock_data::add_strategy_column( ta_helper::ta_strategy i_strategy, std::vector<std::int32_t> i_strategy_column ) noexcept
    {
        m_strategies.try_emplace( i_strategy, i_strategy_column );
    }

    stock_data::iterator stock_data::begin() noexcept
    {
        return m_candles.begin();
    }

    stock_data::iterator stock_data::end() noexcept
    {
        return m_candles.end();
    }

    stock_data::const_iterator stock_data::begin() const noexcept
    {
        return m_candles.begin();
    }

    stock_data::const_iterator stock_data::end() const noexcept
    {
        return m_candles.end();
    }

    const stock_data::candles& stock_data::get_candles() const noexcept
    {
        return ( m_candles );
    }

    std::int32_t stock_data::candle_size() const noexcept
    {
        return m_candle_size;
    }

    raw_stock_input_s stock_data::raw_values() const noexcept
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

    std::string stock_data::columns_str( size_t i_index ) const noexcept
    {
        auto ss{ std::stringstream{} };

        for( auto strategy{ static_cast<ta_helper::ta_strategy>( 0 ) }; strategy < ta_helper::ta_strategy::__LAST;
             strategy = static_cast<ta_helper::ta_strategy>( strategy + 1 ) )
        {
            ss << std::defaultfloat << m_strategies.at( strategy ).at( i_index ) << csv_delimiter;
        }

        return ss.str();
    }
