/**
 * @file string_include.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2023-02-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once

#include "json_include.hpp"
#include "wise_enum_include.hpp"

#include <concepts>
#include <string>

namespace std
{
namespace detail
{
template<typename T>
concept jsonable = requires( T x ) {
                       std::negation_v<std::is_enum<T>>;
                       std::convertible_to<json, T>;
                   };
}

template<typename T>
auto to_string( const T& i_obj )
{
    if constexpr( wise_enum::is_wise_enum_v<T> )
    {
        return std::string{ wise_enum::to_string( i_obj ) };
    }
    else if constexpr( detail::jsonable<T> )
    {
        return std::make_json( i_obj ).dump();
    }
}
}
