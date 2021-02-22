/**
 * @file args_handler_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-28
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <sstream>

#include "gtest/gtest.h"

#include "../test_macros.hpp"

#include "../../src/manager/args_handler.hpp"


TEST(args_handler_tests, invalid_call_test)
{
    auto ss{ std::stringstream{} };

    auto buffer_{ buffer_manager{ss.rdbuf()} };

    auto res{ trading::handle_arguments(0, nullptr) };

    EXPECT_EQ(res, trading_app_result::too_few_arguments);

    EXPECT_EQ(ss.str(), "Too few arguments. Run \"trading --help\" for more information.\n");
}


TEST(args_handler_tests, help_call_test)
{
    auto buffer_{ buffer_manager{} };

    const char* args[] = { "trading",  "--help" };

    auto res{ trading::handle_arguments(2, args) };

    EXPECT_EQ(res, trading_app_result::success);
}
