/**
 * @file processor.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <future>
#include <iostream>
#include <mutex>
#include <string>

#include "ta_handler.hpp"
#include "timer.hpp"

#include "utilities.hpp"
#include "resolution.hpp"
#include "ta_utils.hpp"
#include "processor.hpp"

static std::mutex csv_result_mutex{};

namespace
{
/**
 * @brief Generic processor for command line inputs
 *
 * @tparam _Func template for lambda functions
 * @param i_multi_threaded flag to specify whether to process multiple files at a time
 * @param i_input_path input path for csv files
 * @param i_output_path output path for csv files
 * @param i_process_function processor lambda function
 */
template<typename _Func>
void process( bool i_multi_threaded,
              fs::path i_input_path,
              fs::path i_output_path,
              _Func&& i_process_function )
{
    if( fs::is_directory( i_input_path ) )
    {
        if( !fs::is_directory( i_output_path ) )
        {
            i_output_path = i_output_path.parent_path();
        }

        if( auto file_list{ trading::utilities::find_files( i_input_path.string().c_str(), ".csv" ) };
            !file_list.empty() )
        {
            if( i_multi_threaded )
            {
                auto async_processes{ std::vector<std::future<void>>{} };

                for( auto&& file : file_list )
                {
                    auto out_file{ i_output_path / file.filename() };

                    async_processes.push_back( std::async( std::launch::async, i_process_function, file, out_file ) );
                }
            }
            else
            {
                for( auto&& file : file_list )
                {
                    auto out_file{ i_output_path / file.filename() };

                    i_process_function( file, out_file );
                }
            }
        }
    }
    else if( fs::is_regular_file( i_input_path ) )
    {
        if( fs::is_directory( i_output_path ) )
        {
            i_output_path.append( i_input_path.filename().string() );
        }

        i_process_function( i_input_path, i_output_path );
    }
}

}


namespace trading
{

trading_app_result change_resolution( int i_argc, const char* i_argv[], bool i_multi_threaded )
{
    if( i_argc != 5 )
    {
        return trading_app_result::too_few_arguments;
    }

    const auto new_resolution{ std::stoi( i_argv[2] ) };

    auto process_resolution = [&new_resolution]( const auto& i_input_path, const auto& i_output_path ) {
        auto old_csv_data{ trading::utilities::read_initial_csv( i_input_path ) };

        auto new_csv_data{ trading::csv::resolution::change_resolution( old_csv_data, new_resolution ) };

        trading::utilities::write_csv( new_csv_data, i_output_path, false );
    };

    process( i_multi_threaded, i_argv[3], i_argv[4], process_resolution );

    return trading_app_result::success;
}


trading_app_result identify_patterns( int i_argc, const char* i_argv[], bool i_multi_threaded )
{
    if( i_argc != 5 )
    {
        return trading_app_result::too_few_arguments;
    }

    auto handler{ ta_handler{} };

    auto idx{ std::atomic_int32_t{ 1 } };

    auto csv_result{ strategy_occurrence_count_t{} };

    auto run_patterns = [&idx, &csv_result]( const auto& i_input_path, const auto& i_output_path ) mutable {
        {
            auto m{ trading::utilities::io_lock{} };
            std::cout << std::endl << "File #" << idx++ << std::endl;
        }

        auto csv_data{ trading::utilities::read_initial_csv( i_input_path ) };

        auto num_strategy_occurrences{ trading::ta_utilities::find_patterns( csv_data ) };

        {
            auto lk{ std::lock_guard{ csv_result_mutex } };
            csv_result.emplace_back( i_input_path.stem().string(), std::move( num_strategy_occurrences ) );
        }

        trading::utilities::write_csv( csv_data, i_output_path, true );
    };

    auto tmr{ timer{} };

    process( i_multi_threaded, i_argv[2], i_argv[3], run_patterns );

    trading::utilities::write_strategy_occurrences( i_argv[4], csv_result );

    {
        auto m{ trading::utilities::io_lock{} };
        std::cout << std::endl << "Total time: " << tmr.total_time().count() << "ms" << std::endl;
    }

    return trading_app_result::success;
}
}
