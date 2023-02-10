/**
 * @file task_type.hpp
 * @author ashwinn76
 * @brief Declaration of task_type enumeration
 * @version 0.1
 * @date 2022-07-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "json_include.hpp"
#include "wise_enum_include.hpp"

namespace trading::core
{
WISE_ENUM_CLASS( task_type, unknown, start, pause, stop, buy, sell, status )

NLOHMANN_JSON_SERIALIZE_ENUM( task_type,
                              { { task_type::unknown, nullptr },
                                { task_type::start, std::to_string( task_type::start ) },
                                { task_type::pause, std::to_string( task_type::pause ) },
                                { task_type::stop, std::to_string( task_type::stop ) },
                                { task_type::buy, std::to_string( task_type::buy ) },
                                { task_type::sell, std::to_string( task_type::sell ) },
                                { task_type::status, std::to_string( task_type::status ) } } )

}  // namespace trading::core
