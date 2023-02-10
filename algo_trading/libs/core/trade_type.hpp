/**
 * @file trade_type.hpp
 * @author ashwinn76
 * @brief Declaration of trade_type enum
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "json_include.hpp"
#include "wise_enum_include.hpp"

namespace trading::core
{
/**
 * @brief type of trade to be executed
 *
 */
WISE_ENUM_CLASS( trade_type, unknown, buy, sell )

NLOHMANN_JSON_SERIALIZE_ENUM( trade_type,
                              { { trade_type::unknown, nullptr },
                                { trade_type::buy, std::to_string( trade_type::buy ) },
                                { trade_type::sell, std::to_string( trade_type::sell ) } } )

}  // namespace trading::core
