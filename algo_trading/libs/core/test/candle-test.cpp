/**
 * @file candle-test.cpp
 * @author ashwinn76
 * @brief Tests for candle
 * @version 0.1
 * @date 2022-08-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "libs/core/candle.hpp"

#include "clock.hpp"
#include "json_include.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <string>

using namespace trading;
using namespace trading::core;
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;

namespace
{
auto default_candle() noexcept
{
    return candle{};
}

// Testing values for cpp file
constexpr auto expected_opening_price() noexcept
{
    return 12.4;
}

constexpr auto expected_highest_price() noexcept
{
    return 543.67;
}

constexpr auto expected_lowest_price() noexcept
{
    return 753.756;
}

constexpr auto expected_closing_price() noexcept
{
    return 56.234;
}

auto expected_ticker() noexcept
{
    return "Random"s;
}

constexpr auto expected_timestamp() noexcept
{
    return time_point{ 12ns };
}

auto custom_candle() noexcept
{
    return candle{ expected_ticker(),        expected_timestamp(),    expected_opening_price(),
                   expected_highest_price(), expected_lowest_price(), expected_closing_price() };
}
}

TEST( candleTests, defaultTickerTest )
{
    EXPECT_TRUE( default_candle().ticker().empty() );
}

TEST( candleTests, defaultTimestampTest )
{
    EXPECT_EQ( default_candle().timestamp(), time_point{} );
}

TEST( candleTests, defaultOpeningPriceTest )
{
    EXPECT_DOUBLE_EQ( default_candle().opening_price(), 0.0 );
}

TEST( candleTests, defaultLowestPriceTest )
{
    EXPECT_DOUBLE_EQ( default_candle().lowest_price(), 0.0 );
}

TEST( candleTests, defaultHighestPriceTest )
{
    EXPECT_DOUBLE_EQ( default_candle().highest_price(), 0.0 );
}

TEST( candleTests, defaultClosingPriceTest )
{
    EXPECT_DOUBLE_EQ( default_candle().closing_price(), 0.0 );
}

TEST( candleTests, customTickerTest )
{
    EXPECT_EQ( custom_candle().ticker(), expected_ticker() );
}

TEST( candleTests, customTimestampTest )
{
    EXPECT_EQ( custom_candle().timestamp(), expected_timestamp() );
}

TEST( candleTests, customOpeningPriceTest )
{
    EXPECT_DOUBLE_EQ( custom_candle().opening_price(), expected_opening_price() );
}

TEST( candleTests, customLowestPriceTest )
{
    EXPECT_DOUBLE_EQ( custom_candle().lowest_price(), expected_lowest_price() );
}

TEST( candleTests, customHighestPriceTest )
{
    EXPECT_DOUBLE_EQ( custom_candle().highest_price(), expected_highest_price() );
}

TEST( candleTests, customClosingPriceTest )
{
    EXPECT_DOUBLE_EQ( custom_candle().closing_price(), expected_closing_price() );
}

TEST( candleTests, jsonConversionTest )
{
    json js;
    js["ticker"] = expected_ticker();
    js["timestamp"] = expected_timestamp().time_since_epoch().count();
    js["open"] = expected_opening_price();
    js["high"] = expected_highest_price();
    js["low"] = expected_lowest_price();
    js["close"] = expected_closing_price();

    EXPECT_EQ( custom_candle().to_json(), js );
}
