/**
 * @file python_utils_tests.cpp
 * @author ashwinn76
 * @brief Tests for python_utils.cpp
 * @version 0.1
 * @date 2021-02-07
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "gtest/gtest.h"

#include "helper/python_utils.hpp"


using namespace trading::python;


TEST( python_utils_tests, python_env_tests )
{
    auto p{ init_python_env() };

    EXPECT_EQ( p.use_count(), 2L );

    EXPECT_TRUE( Py_IsInitialized() );
}
