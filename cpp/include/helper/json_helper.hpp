/**
 * @file json_helper.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-19
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <sstream>
#include <string>

#include <nlohmann/json.hpp>

using namespace nlohmann;


namespace std
{
json stojson( const std::string& i_str );

std::string to_string( const json& i_json );
}