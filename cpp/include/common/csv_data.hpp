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

#include <filesystem>
#include <list>
#include <unordered_map>

#include "structs.hpp"
#include "stock_data.hpp"


class csv_data
{
public:
    std::filesystem::path m_filepath{};  // Name of file from which csv data has been loaded

    std::list<date_s> dates{};  // List of unique dates

    std::unordered_map<date_s, stock_data> stock_map{};  // Map of date to stock data

    class iterator
    {
    private:
        const csv_data& m_data;
        std::list<date_s>::const_iterator m_iter{};

    public:
        iterator( const csv_data& data ) noexcept : m_data{ data }, m_iter{ data.dates.begin() }
        {
        }

        typename decltype( stock_map )::const_reference operator*() const noexcept
        {
            return *m_data.stock_map.find( *m_iter );
        }

        iterator& operator++() noexcept
        {
            ++m_iter;
            return *this;
        }
    };
};