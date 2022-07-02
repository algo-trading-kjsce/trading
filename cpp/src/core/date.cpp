/**
 * @file date.cpp
 * @author ashwinn76
 * @brief definition of date member functions
 * @version 0.1
 * @date 2022-06-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common/includes.hpp"

#include "date.hpp"

#include <iomanip>
#include <sstream>

std::string date_s::to_string() const{
            auto ss{ std::stringstream{} };

        ss << std::setw( 2 ) << std::setfill( '0' ) << m_year << date_delimiter << std::setw( 2 ) << std::setfill( '0' )
           << static_cast<std::int32_t>( m_month ) << date_delimiter << std::setw( 2 ) << std::setfill( '0' ) << m_day;

        return ss.str();

}

    std::optional<date_s> try_create_date( const std::string& i_string )
    {
        auto o_date{ std::optional<date_s>{} };

        if( !i_string.empty() )
        {
            o_date.emplace();

            auto ss{ std::stringstream{ i_string } };

            auto parsed{ std::string{} };

            if( std::getline( ss, parsed, date_delimiter ) )
            {
                o_date->m_year = std::stoi( parsed );
            }
            else
            {
                o_date.reset();
            }

            if( std::getline( ss, parsed, date_delimiter ) && o_date.has_value() )
            {
                o_date->m_month = static_cast<month>( std::stoi( parsed ) );
            }
            else
            {
                o_date.reset();
            }

            if( std::getline( ss, parsed, date_delimiter ) && o_date.has_value() )
            {
                o_date->m_day = std::stoi( parsed );
            }
            else
            {
                o_date.reset();
            }
        }

        return o_date;
    }
