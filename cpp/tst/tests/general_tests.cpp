/**
 * @file general_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-11-05
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "gtest/gtest.h"

#include <future>

#include "../test_macros.hpp"

#include "../../src/processor/utilities.hpp"

auto test_floating_point_container_equality(const std::vector<double>& c1, const std::vector<double>& c2, int ln)
{
    EXPECT_EQ(c1.size(), c2.size());

    for (auto i{ 0_sz }; i < c1.size(); ++i)
    {
        EXPECT_TRUE(abs(c1[i] - c2[i]) < 1e-4) << i + 1 << "th element did not match on line " << ln << " c1: " << c1[i] << ", c2: " << c2[i];
    }
}

TEST(general_tests, stock_data_raw_values_single_thread)
{
    auto buffer_{ buffer_manager{} };

    auto directory_path{ TEST_DATA_DIRECTORY };

    auto csv_5min_file{ directory_path / "raw_values_test_0.csv" };

    auto csv_data{ trading::utilities::read_initial_csv(csv_5min_file) };

    auto raw_values{ csv_data.stock_map.at(date_s{2015, month::feb, day{4}}).raw_values() };

    auto expected_opens{ std::vector<double>{475.05, 472.95, 473.4, 474.95, 476, 476.65, 475.5, 475.65, 473.65,
                                             474.3, 474.05, 474.5, 474.15, 474.2, 474, 473.8, 473.45, 473.75,
                                             472.85, 473.45, 473.7, 472.8, 473.1, 473.45, 473.6, 473.75, 473.8,
                                             473.45, 473.75, 473.65, 473.75, 473.75, 473.7, 473.7, 474.45, 474.7,
                                             474.95, 474.65, 474.75, 474.6, 473.8, 474.2, 474.7, 474, 474.25,
                                             474.45, 474.75, 475, 474.75, 474.9, 474.45, 474.3, 474.45, 474.95,
                                             474.95, 475.1, 474.25, 474.75, 474.8, 473.85, 474.95, 474.35, 474.7,
                                             474.8, 474.95, 474.95, 474.75, 474.85, 474.6, 474.95, 475, 475, 475,
                                             474.95, 475.5} };

    test_floating_point_container_equality(raw_values.opens, expected_opens, __LINE__);

    raw_values = csv_data.stock_map.at(date_s{ 2015, month::feb, day{6} }).raw_values();

    auto expected_highs{ std::vector<double>{499, 499, 496.25, 496.8, 495.6, 496.15, 495.5, 494.85, 493.75,
                                             493.75, 495, 494.85, 495.5, 495.25, 494.95, 494.9, 494.25, 490.6,
                                             489.85, 489.25, 489.75, 491.85, 491.75, 492.35, 492.05, 491.5,
                                             491.6, 492, 491.2, 490.85, 490.4, 490.25, 490.05, 488.85, 489.05,
                                             489, 489.75, 489.75, 490.25, 489.95, 489.75, 489.9, 490, 490.6,
                                             491.1, 490.95, 490.55, 490.2, 489.9, 490.25, 489.95, 489.85, 489.25,
                                             489.45, 490, 489.85, 490.7, 490.1, 490.5, 490.2, 489.7, 491.05,
                                             490.95, 490, 489.95, 489.5, 490.5, 490.65, 490, 489.1, 489.6, 488.5,
                                             489.2, 490, 490.5} };

    test_floating_point_container_equality(raw_values.highs, expected_highs, __LINE__);
}


TEST(general_tests, stock_data_raw_values_multi_thread)
{
    auto buffer{ buffer_manager{} };

    auto directory_path{ TEST_DATA_DIRECTORY };

    auto processes{ std::vector<std::future<csv_data>>{} };
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_0.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_1.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_2.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_3.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_4.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_5.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_6.csv"));
    processes.push_back(std::async(std::launch::async, trading::utilities::read_initial_csv, directory_path / "raw_values_test_7.csv"));


    for (auto&& process : processes)
    {
        process.wait();
    }

    auto first_csv{ processes.at(0_sz).get() };
    auto second_csv{ processes.at(1_sz).get() };
    auto third_csv{ processes.at(2_sz).get() };
    auto fourth_csv{ processes.at(3_sz).get() };

    auto same_as_fourth_csv{ processes.at(4_sz).get() };
    auto same_as_third_csv{ processes.at(5_sz).get() };
    auto same_as_second_csv{ processes.at(6_sz).get() };
    auto same_as_first_csv{ processes.at(7_sz).get() };

    auto first_csv_doubles{ std::vector{474.15, 472.8, 475.0, 475.1, 476.95, 481.45, 480.75, 480.8, 480.5,
                                        481.7, 481.8, 482.25, 483.35, 484.15, 482.95, 483.25, 483.6, 484.75,
                                        485.05, 484.15, 484.7, 484.6, 483.45, 484.15, 484.9, 485.85, 485.95,
                                        486.4, 487.6, 487.85, 488.65, 487.55, 487.8, 487.75, 487.15, 487.3,
                                        487.35, 487.2, 487.75, 487.7, 487.45, 487.15, 487.5, 487.4, 487.95,
                                        488.0, 488.0, 489.0, 490.75, 491.15, 491.45, 491.5, 489.65, 490.05,
                                        490.4, 490.25, 490.35, 491.65, 492.1, 492.35, 492.2, 492.0, 490.85,
                                        492.55, 492.9, 493.0, 493.35, 494.6, 494.3, 493.15, 493.7, 492.9,
                                        493.45, 493.45, 493.55} };

    auto raw_values{ first_csv.stock_map.at({2015, month::feb, day{5}}).raw_values() };

    test_floating_point_container_equality(raw_values.opens, first_csv_doubles, __LINE__);

    auto raw_values_1{ third_csv.stock_map.at({2018, month::aug, day{20}}).raw_values() };
    auto same_raw_values{ same_as_third_csv.stock_map.at({2018, month::aug, day{20}}).raw_values() };

    test_floating_point_container_equality(raw_values_1.opens, same_raw_values.opens, __LINE__);
    test_floating_point_container_equality(raw_values_1.highs, same_raw_values.highs, __LINE__);
    test_floating_point_container_equality(raw_values_1.lows, same_raw_values.lows, __LINE__);
    test_floating_point_container_equality(raw_values_1.closes, same_raw_values.closes, __LINE__);
}

