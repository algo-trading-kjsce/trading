/**
 * @file args_handler.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <cstdint>

#include "enums.hpp"

namespace trading
{
/**
 * @brief Handler for the input arguments given to trading app
 *
 * @param argc number of arguments
 * @param argv arguments
 * @return result of operation execution
 */
trading_app_result handle_arguments( std::int32_t argc, const char* argv[] );
}