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

#include <iostream>
#include <string.h>

#include <mutex>
#include <future>

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
 * @brief Output for invalid argument passed in
 *
 */
void invalid_arg()
{
    std::cout << "Too few arguments. Run \"trading --help\" for more information." << std::endl;
}


/**
 * @brief Show the help output in terminal
 *
 */
void show_help()
{
    std::cout << std::endl;

    std::cout << R"(                                                          --- Trading App ---

    trading -r <new_res> <input_file> <output_path>              = Change resolution of csv files. Input can be one csv file or a directory
    trading -p <input_file> <output_path> <pattern_report_path>  = For running pattern recognition from TA-Lib. Input can be one csv file or a directory
)";
}


/**
 * @brief Generic processor for command line inputs
 *
 * @tparam _Func template for lambda functions
 * @param i_multi_threaded flag to specify whether to process multiple files at a time
 * @param i_input_path input path for csv files
 * @param i_output_path output path for csv files
 * @param i_process_function processor lambda function
 */
template <typename _Func>
void process(bool i_multi_threaded, std::filesystem::path i_input_path, std::filesystem::path i_output_path, _Func&& i_process_function)
{
    if (std::filesystem::is_directory(i_input_path))
    {
        if (!std::filesystem::is_directory(i_output_path))
        {
            i_output_path = i_output_path.parent_path();
        }

        if (auto file_list{ trading::utilities::find_files(i_input_path.string().c_str(), ".csv") }; !file_list.empty())
        {
            if (i_multi_threaded)
            {
                auto async_processes{ std::vector<std::future<void>>{} };

                for (auto&& file : file_list)
                {
                    auto out_file{ i_output_path / file.filename() };

                    async_processes.push_back(std::async(std::launch::async, i_process_function, file, out_file));
                }
            }
            else
            {
                for (auto&& file : file_list)
                {
                    auto out_file{ i_output_path / file.filename() };

                    i_process_function(file, out_file);
                }
            }

        }
    }
    else if (std::filesystem::is_regular_file(i_input_path))
    {
        if (std::filesystem::is_directory(i_output_path))
        {
            i_output_path.append(i_input_path.filename().string());
        }

        i_process_function(i_input_path, i_output_path);
    }
}


auto change_resolution(int i_argc, const char* i_argv[], bool i_multi_threaded)
{
    if (i_argc != 5)
    {
        invalid_arg();
        return trading::trading_app_result::too_few_arguments;
    }

    const auto new_resolution{ std::stoi(i_argv[2]) };

    auto process_resolution = [&new_resolution](const auto& i_input_path, const auto& i_output_path)
    {
        auto old_csv_data{ trading::utilities::read_initial_csv(i_input_path) };

        auto new_csv_data{ trading::csv::resolution::change_resolution(old_csv_data, new_resolution) };

        trading::utilities::write_csv(new_csv_data, i_output_path, false);
    };

    process(i_multi_threaded, i_argv[3], i_argv[4], process_resolution);

    return trading::trading_app_result::success;
}


auto identify_patterns(int i_argc, const char* i_argv[], bool i_multi_threaded)
{
    if (i_argc != 5)
    {
        invalid_arg();
        return trading::trading_app_result::too_few_arguments;
    }

    auto handler{ ta_handler{} };

    auto idx{ std::atomic_int32_t{1} };

    auto csv_result{ strategy_occurrence_count_t{} };

    auto run_patterns = [&idx, &csv_result](const auto& i_input_path, const auto& i_output_path) mutable
    {
        {
            auto m{ trading::utilities::io_lock{} };
            std::cout << std::endl << "File #" << idx++ << std::endl;
        }

        auto csv_data{ trading::utilities::read_initial_csv(i_input_path) };

        auto num_strategy_occurrences{ trading::ta_utilities::find_patterns(csv_data) };

        {
            auto lk{ std::lock_guard{csv_result_mutex} };
            csv_result.emplace_back(i_input_path.stem().string(), std::move(num_strategy_occurrences));
        }

        trading::utilities::write_csv(csv_data, i_output_path, true);
    };

    auto tmr{ timer{} };

    process(i_multi_threaded, i_argv[2], i_argv[3], run_patterns);

    trading::utilities::write_strategy_occurrences(i_argv[4], csv_result);

    {
        auto m{ trading::utilities::io_lock{} };
        std::cout << std::endl << "Total time: " << tmr.total_time().count() << "ms" << std::endl;
    }

    return trading::trading_app_result::success;
}

}


namespace trading
{

trading_app_result handle_arguments(std::int32_t argc, const char* argv[])
{
    auto result{ trading_app_result{} };

    switch (argc)
    {

    case 0:
    case 1:
    {
        invalid_arg();

        result = trading_app_result::too_few_arguments;
    }
    break;

    case 2:
    {
        if (strcmp(argv[1], "--help") == 0)
        {
            show_help();
        }
        else
        {
            invalid_arg();
        }
    }
    break;

    default:
    {
        auto arg1{ std::string{argv[1]} };

        if (arg1 == "-r")
        {
            result = change_resolution(argc, argv, false);
        }
        else if (arg1 == "-mr")
        {
            result = change_resolution(argc, argv, true);
        }
        else if (arg1 == "-p")
        {
            result = identify_patterns(argc, argv, false);
        }
        else if (arg1 == "-mp")
        {
            result = identify_patterns(argc, argv, true);
        }
        else
        {
            invalid_arg();
        }
    }
    break;
    }

    return result;
}

}
