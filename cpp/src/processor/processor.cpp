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

#include "ta_handler.hpp"
#include "timer.hpp"


#include "utilities.hpp"
#include "resolution.hpp"
#include "ta_utils.hpp"
#include "processor.hpp"



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


template <typename _Func>
void process(std::filesystem::path i_input_path, std::filesystem::path i_output_path, _Func&& i_process_function)
{
    if (std::filesystem::is_directory(i_input_path))
    {
        if (!std::filesystem::is_directory(i_output_path))
        {
            i_output_path = i_output_path.parent_path();
        }

        for (auto&& file : trading::utilities::find_files(i_input_path.string().c_str(), ".csv"))
        {
            auto out_file{ i_output_path / file.filename() };

            i_process_function(file, out_file);
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
        if (strcmp(argv[1], "-r") == 0)
        {
            const auto new_resolution{ std::stoi(argv[2]) };

            auto process_resolution = [&new_resolution](const auto& i_input_path, const auto& i_output_path)
            {
                auto old_csv_data{ trading::utilities::read_initial_csv(i_input_path) };

                auto new_csv_data{ trading::csv::resolution::change_resolution(old_csv_data, new_resolution) };

                trading::utilities::write_csv(new_csv_data, i_output_path, false);
            };

            process(argv[3], argv[4], process_resolution);
        }
        else if (strcmp(argv[1], "-p") == 0)
        {
            auto handler{ ta_handler{} };

            auto idx{ 1 };

            auto csv_result{ strategy_occurrence_count_t{} };

            auto run_patterns = [&idx, &csv_result](const auto& i_input_path, const auto& i_output_path) mutable
            {
                std::cout << "\nFile #" << idx++ << std::endl;

                auto csv_data{ trading::utilities::read_initial_csv(i_input_path) };

                csv_result.emplace_back(i_input_path.stem().string(), trading::ta_utilities::find_patterns(csv_data));

                trading::utilities::write_csv(csv_data, i_output_path, true);
            };

            auto tmr{ timer{} };

            process(argv[2], argv[3], run_patterns);

            trading::utilities::write_strategy_occurrences(argv[4], csv_result);

            std::cout << std::endl << "Total time: " << tmr.total_time().count() << "ms" << std::endl;
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
