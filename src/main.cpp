/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "timer.hpp"
#include "utilities.hpp"
#include "ta_utils.hpp"

int main()
{
    auto handler{ ta_utilities::ta_handler{} };

    auto csv_files{ utilities::find_files("/home/ashwinn76/Downloads/CSV/", ".csv") };

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

    utilities::write_results("/home/ashwinn76/Downloads/result.csv", csv_result, strategy_str);
}