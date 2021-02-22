/**
 * @file json_helper.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-19
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "json_helper.hpp"

namespace std
{
json stojson( const std::string& i_str )
{
    auto js{ json{} };
    if( auto stream{ std::stringstream{ i_str } }; stream.good() )
    {
        stream >> js;
    }

    return js;
}

std::string to_string( const json& i_json )
{
    auto stream{ std::ostringstream{} };

    stream << i_json;

    return stream.str();
}
}
