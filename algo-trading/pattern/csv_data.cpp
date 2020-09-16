/**
 * @file csv_data.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "../include/includes.hpp"
#include "csv_data.hpp"


void csv_data_s::add_custom_column_name(std::string i_column_name) noexcept
{
    csv_custom_column_names += i_column_name;
    csv_custom_column_names += delimiter;

}


void csv_data_s::add_strategy_column_name(std::string i_column_name) noexcept
{
    csv_strategy_column_names += i_column_name;
    csv_strategy_column_names += delimiter;
}


std::string csv_data_s::get_header() const noexcept
{
    auto header_str{ csv_column_names };

    if (!(header_str.back() == delimiter))
    {
        header_str += delimiter;
    }

    header_str.append(csv_custom_column_names);

    if (!(header_str.back() == delimiter))
    {
        header_str += delimiter;
    }

    header_str.append(csv_strategy_column_names);

    return header_str;
}
