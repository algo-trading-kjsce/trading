/**
 * @file node_info-test.cpp
 * @author ashwinn76
 * @brief node_info tests
 * @version 0.1
 * @date 2022-08-19
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/node_info.hpp"

#include <gtest/gtest.h>

using namespace trading::core;

namespace
{
auto default_node_info() noexcept
{
    return node_info{};
}
}

TEST( nodeInfoTests, defaultTypeTest )
{
    EXPECT_EQ( default_node_info().type, node_type::unknown );
}

TEST( nodeInfoTests, defaultLoggerTest )
{
    EXPECT_FALSE( default_node_info().logger.has_value() );
}

TEST( nodeInfoTests, defaultKillFlagTest )
{
    EXPECT_FALSE( default_node_info().kill_flag.has_value() );
}

TEST(nodeInfoTests, defaultMessagePublisherTest)
{
    //    EXPECT_EQ( default_node_info().message_publisher, nullptr );
}
