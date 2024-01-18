/**
 * @file jsnon_include.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2022-06-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace nlohmann
{
template<typename T>
void to_json( json& j, const std::optional<T>& opt )
{
    if( opt.has_value() )
    {
        j = opt.value();
    }
    else
    {
        j = nullptr;
    }
}

template<typename T>
void from_json( const json& j, std::optional<T>& opt )
{
    if( j.is_null() )
    {
        opt = std::nullopt;
    }
    else
    {
        opt = j.get<T>();
    }
}
}

namespace std
{

template<typename T>
auto make_json( const T& i_obj )
{
    return json( i_obj );
}

}
