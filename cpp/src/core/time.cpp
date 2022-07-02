/**
 * @file time.cpp
 * @author ashwinn76
 * @brief definition of time member functions
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common/includes.hpp"

#include "time.hpp"

#include <iomanip>
#include <sstream>

std::string time_s::to_string() const
{
            auto ss{ std::stringstream{} };

        ss << std::setw( 2 ) << std::setfill( '0' ) << hours << time_delimiter << std::setw( 2 ) << std::setfill( '0' )
           << minutes << time_delimiter << std::setw( 2 ) << std::setfill( '0' ) << seconds;

        return ss.str();
}

    void time_s::add_minutes( std::int32_t i_minutes ) noexcept
    {
        minutes += i_minutes;

        if( minutes >= 60 )
        {
            hours += minutes / 60;
            minutes %= 60;
        }
    }

        std::optional<time_s> try_create_time( const std::string& i_string )
    {
        auto o_time{ std::optional<time_s>{} };

        if( !i_string.empty() )
        {
            o_time.emplace();

            auto ss{ std::stringstream{ i_string } };

            auto parsed{ std::string{} };

            if( std::getline( ss, parsed, time_delimiter ) )
            {
                o_time->hours = std::stoi( parsed );
            }
            else
            {
                o_time.reset();
            }

            if( std::getline( ss, parsed, time_delimiter ) && o_time.has_value() )
            {
                o_time->minutes = std::stoi( parsed );
            }

            if( std::getline( ss, parsed, time_delimiter ) && o_time.has_value() )
            {
                o_time->seconds = std::stoi( parsed );
            }
        }

        return o_time;
    }
