/**
 * @file preprocessor.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <cstdint>

namespace trading
{

enum class trading_app_result
{
    success,
    passive_failure,
    too_few_arguments,
    unknown_error,
};


/**
 * @brief Handler for the input arguments given to trading app
 *
 * @param argc number of arguments
 * @param argv arguments
 * @return result of operation execution
 */
trading_app_result handle_arguments(std::int32_t argc, const char* argv[]);

}