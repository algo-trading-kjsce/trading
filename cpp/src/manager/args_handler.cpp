/**
 * @file args_handler.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-02-11
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <iostream>
#include <string>
#include <vector>

#include "processor/processor.hpp"

#include "args_handler.hpp"
#include "trading_manager.hpp"

namespace
{
/**
 * @brief Show the help output in terminal
 *
 */
void show_help()
{
    std::cout << R"(
                                                          --- Trading App ---

    trading -r <new_res> <input_file> <output_path>              = Change resolution of csv files. Input can be one csv file or a directory
    trading -p <input_file> <output_path> <pattern_report_path>  = For running pattern recognition from TA-Lib. Input can be one csv file or a directory
    trading -t <python_scripts_directory>                        = For starting day trading algorithms and pattern recognition 
)";
}


/**
 * @brief Output for invalid number of arguments
 *
 */
void invalid_arg()
{
    std::cout << "Too few arguments. Run \"trading --help\" for more information.\n";
}

}


namespace trading
{
trading_app_result handle_arguments( std::int32_t argc, const char* argv[] )
{
    auto result{ trading_app_result{} };

    switch( argc )
    {
    case 0:
    case 1: {
        invalid_arg();

        result = trading_app_result::too_few_arguments;
    }
    break;

    case 2: {
        if( strcmp( argv[1], "--help" ) == 0 )
        {
            show_help();
        }
        else
        {
            invalid_arg();
        }
    }
    break;

    default: {
        auto arg1{ std::string{ argv[1] } };

        if( arg1 == "-r" )
        {
            result = trading::change_resolution( argc, argv, false );
        }
        else if( arg1 == "-mr" )
        {
            result = trading::change_resolution( argc, argv, true );
        }
        else if( arg1 == "-p" )
        {
            result = trading::identify_patterns( argc, argv, false );
        }
        else if( arg1 == "-mp" )
        {
            result = trading::identify_patterns( argc, argv, true );
        }
        else if( arg1 == "-t" )
        {
            auto python_script_paths{ std::vector<std::string>{} };

            auto arg_index{ 2 };
            for( ; arg_index < argc; ++arg_index )
            {
                auto current_arg{ std::string{ argv[arg_index] } };

                if( current_arg == "-s" )
                {
                    arg_index++;
                    break;
                }

                python_script_paths.push_back( std::move( current_arg ) );
            }

            auto stocks{ std::vector<std::string>{} };
            for( ; arg_index < argc; ++arg_index )
            {
                stocks.emplace_back( argv[arg_index] );
            }

            trading::run_trading_manager( std::move( python_script_paths ), std::move( stocks ) );
        }
        else
        {
            invalid_arg();
        }

        if( result == trading_app_result::too_few_arguments )
        {
            invalid_arg();
        }
    }
    break;
    }

    return result;
}
}
