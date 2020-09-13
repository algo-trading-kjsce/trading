/**
 * @file csv_data.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <string>
#include <list>
#include <unordered_map>

#include "stock_data.hpp"


struct csv_data_s
{
    std::list<date_s> dates{};    // List of unique dates

    std::unordered_map<date_s, stock_data> stock_map{};  // Map of date to stock data

    std::string csv_column_names{};   // Column names in csv format
    std::string csv_custom_column_names{};    // Columns that are added on later
    std::string csv_strategy_column_names{};    // Strategy columns

    /**
     * @brief add name of custom column
     *
     * @param i_column_name name of the custom column
     */
    void add_custom_column_name(std::string i_column_name) noexcept;


    /**
     * @brief add name of strategy column
     *
     * @param i_column_name name of the strategy column
     */
    void add_strategy_column_name(std::string i_column_name) noexcept;
    
    
    /**
     * @brief Get the header string for csv file
     * 
     * @return std::string 
     */
    std::string get_header() const noexcept;
};