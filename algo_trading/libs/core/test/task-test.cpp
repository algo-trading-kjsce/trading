/**
 * @file task-test.cpp
 * @author ashwinn76
 * @brief Tests for task class
 * @version 0.1
 * @date 2022-09-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/task.hpp"

#include <gtest/gtest.h>

using namespace trading;
using namespace trading::core;

using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;

namespace
{
constexpr auto expected_task_type() noexcept
{
    return task_type::pause;
}

constexpr auto expected_node_type() noexcept
{
    return ros::node_type::planner;
}

constexpr auto expected_time_stamp() noexcept
{
    return time_point{ 324ns };
}

auto expected_tickers() noexcept
{
    return std::vector{ "T"s, "AMD"s, "ADBE"s };
}

auto test_task()
{
    return task{ expected_task_type(), expected_node_type(), expected_time_stamp(), expected_tickers() };
}
}

TEST( taskTest, taskTypeTest )
{
    EXPECT_EQ( test_task().type(), expected_task_type() );
}

TEST( taskTest, nodeTypeTest )
{
    EXPECT_EQ( test_task().node(), expected_node_type() );
}

TEST( taskTest, timestampTest )
{
    EXPECT_EQ( test_task().timestamp(), expected_time_stamp() );
}

TEST( taskTest, tickersTest )
{
    EXPECT_EQ( test_task().tickers(), expected_tickers() );
}

TEST( taskTest, jsonConversionTest )
{
    json js;
    js["type"] = expected_task_type();
    js["node"] = expected_node_type();
    js["timestamp"] = expected_time_stamp().time_since_epoch().count();
    js["tickers"] = expected_tickers();

    EXPECT_EQ( test_task().to_json(), js );
}
