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

#include "ta_handler.hpp"
#include "timer.hpp"
#include "utilities.hpp"
#include "ta_utils.hpp"

int main([[maybe_unused]] int argc, char* argv[])
{
    auto handler{ ta_handler{} };

#ifdef _WIN32
    auto csv_files{ utilities::find_files(argv[1], argv[2]) };
#else
    auto csv_files{ utilities::find_files(argv[0], argv[1]) };
#endif

    auto csv_result{ csv_result_t{} };

    auto strategy_str{ std::string{} };

    for (auto&& csv_file : csv_files)
    {
        auto csv_data{ utilities::read_csv(csv_file) };

        csv_result.emplace_back(csv_file.stem().string(), ta_utilities::find_patterns(csv_data));

        if (strategy_str.empty())
        {
            strategy_str = csv_data.csv_strategy_column_names;
        }

        utilities::write_csv(csv_data, csv_file);
    }

#ifdef _WIN32
    utilities::write_results(argv[3], csv_result, strategy_str);
#else
    utilities::write_results(argv[2], csv_result, strategy_str);
#endif
}