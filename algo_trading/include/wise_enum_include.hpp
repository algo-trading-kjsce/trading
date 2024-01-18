/**
 * @file wise_enum_include.hpp
 * @author ashwinn76
 * @brief Extension of wise_enum for personal use
 * @version 0.1
 * @date 2022-09-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "wise_enum/wise_enum.h"

#include <concepts>
#include <exception>
#include <string>
#include <type_traits>

namespace std
{

namespace detail
{
template<typename T>
concept wise_enum_type = requires( T a ) {
                             std::is_enum_v<T>;
                             wise_enum::is_wise_enum_v<T>;
                         };
}

template<detail::wise_enum_type T>
auto to_enum( const std::string& i_str )
{
    const auto enum_value{ wise_enum::from_string<T>( i_str ) };

    if( !enum_value.has_value() )
    {
        throw std::runtime_error{ "Invalid string for conversion to enumeration!" };
    }

    return enum_value.value();
}

}
