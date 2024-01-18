/**
 * @file node_type.hpp
 * @author ashwinn76
 * @brief Declaration of node_type enum
 * @version 0.1
 * @date 2022-08-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "json_include.hpp"
#include "string_include.hpp"
#include "wise_enum_include.hpp"

namespace trading::core
{
WISE_ENUM_CLASS( node_type, unknown, messenger, translator, planner, trader )

NLOHMANN_JSON_SERIALIZE_ENUM( node_type,
                              { { node_type::unknown, nullptr },
                                { node_type::messenger, std::to_string( node_type::messenger ) },
                                { node_type::translator, std::to_string( node_type::translator ) },
                                { node_type::planner, std::to_string( node_type::planner ) },
                                { node_type::trader, std::to_string( node_type::trader ) } } )

}  // namespace trading::core
