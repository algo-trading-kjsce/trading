/**
 * @file trade_request-test.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/trade_request.hpp"

#include <gtest/gtest.h>

using namespace trading;
using namespace trading::core;
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;

namespace
{
constexpr auto expected_timestamp() noexcept
{
    return time_point{ 76ns };
}

auto expected_ticker() noexcept
{
    return "TSLA"s;
}

constexpr auto expected_trade_type() noexcept
{
    return trade_type::sell;
}

constexpr auto expected_price() noexcept
{
    return 23.1;
}

auto test_request() noexcept
{
    return trade_request{ expected_timestamp(), expected_ticker(), expected_trade_type(), expected_price() };
}
}

TEST( tradeRequestTest, timestampTest )
{
    EXPECT_EQ( test_request().timestamp(), expected_timestamp() );
}

TEST( tradeRequestTest, tickerTest )
{
    EXPECT_EQ( test_request().ticker(), expected_ticker() );
}

TEST( tradeRequestTest, tradeTypeTest )
{
    EXPECT_EQ( test_request().type(), expected_trade_type() );
}

TEST( tradeRequestTest, priceTest )
{
    EXPECT_DOUBLE_EQ( test_request().price(), expected_price() );
}
