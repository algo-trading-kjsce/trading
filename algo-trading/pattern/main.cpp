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

#include <iostream>

#include "ta_handler.hpp"
#include "timer.hpp"

#include "../csv_utilities/utilities.hpp"

#include "ta_utils.hpp"


int main([[maybe_unused]] int argc, char* argv[])
{
    auto handler{ ta_handler{} };

    auto csv_files{ utilities::find_files(argv[1], argv[2]) };

    auto csv_result{ strategy_occurrence_count_t{} };

    auto tmr{ timer{} };

    auto idx{ 1 };

    for (auto&& csv_file : csv_files)
    {
        std::cout << "\nFile #" << idx++ << '\n';

        auto csv_data{ utilities::read_initial_csv(csv_file) };

        csv_result.emplace_back(csv_file.stem().string(), ta_utilities::find_patterns(csv_data));

        utilities::write_csv_with_strategies(csv_data, csv_file);
    }

    std::cout << "\nTotal time: " << tmr.total_time().count() << "ms\n";

    utilities::write_strategy_occurrences(argv[3], csv_result);
}