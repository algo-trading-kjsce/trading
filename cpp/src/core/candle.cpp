/**
 * @file candle.cpp
 * @author ashwinn76
 * @brief definition of candle member functions
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common/includes.hpp"

#include "candle.hpp"

#include <sstream>

    candle_direction candle_s::direction() const noexcept
    {
        if( open == close )
        {
            return candle_direction::stable;
        }
        else if( open > close )
        {
            return candle_direction::down;
        }
        else if (close > open)
        {
            return candle_direction::up;
        }
        else{return candle_direction::none;}
    }

    void candle_s::write_csv_text( std::ostream& io_stream ) const
    {
        io_stream << index << csv_delimiter << date.to_string() << ' ' << time.to_string() << csv_delimiter << open
                  << csv_delimiter << high << csv_delimiter << low << csv_delimiter << close << csv_delimiter << volume;
    }

    std::optional<candle_s> try_create_candle( const std::string& i_line )
    {
        auto o_candle{ std::optional<candle_s>{} };

        if( !i_line.empty() )
        {
            o_candle.emplace();

            auto stream{ std::stringstream{ i_line } };

            auto parsed{ std::string{} };

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                o_candle->index = std::stoi( parsed );
            }
            else
            {
                o_candle.reset();
            }

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                auto ss{ std::stringstream{ parsed } };

                if( std::getline( ss, parsed, ' ' ) )
                {
                    if( auto o_date{ try_create_date( parsed ) }; o_date.has_value() )
                    {
                        o_candle->date = std::move( o_date.value() );
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

                if( std::getline( ss, parsed, ' ' ) && o_candle.has_value() )
                {
                    if( auto o_time{ try_create_time( parsed ) }; o_time.has_value() )
                    {
                        o_candle->time = std::move( o_time.value() );
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

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                o_candle->open = std::stod( parsed );
            }
            else
            {
                o_candle.reset();
            }

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                o_candle->high = std::stod( parsed );
            }
            else
            {
                o_candle.reset();
            }

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                o_candle->low = std::stod( parsed );
            }
            else
            {
                o_candle.reset();
            }

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                o_candle->close = std::stod( parsed );
            }
            else
            {
                o_candle.reset();
            }

            if( std::getline( stream, parsed, csv_delimiter ) && o_candle.has_value() )
            {
                o_candle->volume = std::stoi( parsed );
            }
            else
            {
                o_candle.reset();
            }
        }

        return o_candle;
    }
