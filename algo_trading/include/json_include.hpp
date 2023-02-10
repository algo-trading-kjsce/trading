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

namespace std
{

template<typename T>
auto make_json( const T& i_obj )
{
    return json( i_obj );
}

}
