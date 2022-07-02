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

#include "common/json_include.hpp"

#include <string>

namespace std
{

json stojson( const std::string& i_str );

std::string to_string( const json& i_json );

}
