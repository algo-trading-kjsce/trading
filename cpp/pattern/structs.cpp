/**
 * @file structs.cpp
 * @author ashwinn76
 * @brief 
 * @version 0.1
 * @date 2020-09-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <sstream>
#include <iomanip>
#include "includes.hpp"
#include "structs.hpp"


std::string date_s::to_str() const noexcept
{
    auto ss{ std::stringstream{} };

    ss <<
        std::setw(2) << std::setfill('0') << m_year << '-' <<
        std::setw(2) << std::setfill('0') << static_cast<int>(m_month) << '-' <<
        std::setw(2) << std::setfill('0') << m_day;

    return ss.str();
}


bool operator==(const date_s& i_lhs, const date_s& i_rhs) noexcept
{
    return i_lhs.m_year == i_rhs.m_year &&
        i_lhs.m_month == i_rhs.m_month &&
        i_lhs.m_day == i_rhs.m_day;
}


std::string time_s::to_str() const noexcept
{
    auto ss{ std::stringstream{} };

    ss <<
        std::setw(2) << std::setfill('0') << hours << ':' <<
        std::setw(2) << std::setfill('0') << minutes << ':' <<
        std::setw(2) << std::setfill('0') << seconds;

    return ss.str();
}


std::ostream& operator<<(std::ostream& io_stream, const candle_s& i_candle)
{
    io_stream
        << i_candle.index << delimiter << i_candle.date.to_str() << ' ' << i_candle.time.to_str()
        << delimiter << i_candle.open << delimiter << i_candle.high << delimiter << i_candle.low << delimiter << i_candle.close
        << delimiter << i_candle.volume;
    
    return io_stream;
}
