/**
 * @file main.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "../manager/args_handler.hpp"

int main( int argc, const char* argv[] )
{
    auto result{ trading::handle_arguments( argc, argv ) };

    return result == trading_app_result::success ? 0 : 1;
}
