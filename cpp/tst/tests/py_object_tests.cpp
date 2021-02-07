/**
 * @file py_object_tests.cpp
 * @author ashwinn76
 * @brief Tests for py_object class
 * @version 0.1
 * @date 2021-02-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "gtest/gtest.h"

#include "../test_macros.hpp"

#include "type_trait_utils.hpp"

#include "pylib/py_object.hpp"

using namespace trading::python;

TEST(py_object_tests, constructor_tests)
{
    auto p1{ py_object{} };

    EXPECT_EQ(p1.get_pointer(), nullptr);

    auto p2{ py_object{PyUnicode_FromString("Hi, this is a random string!")} };

    EXPECT_EQ(p2.ref_count(), 1_sz);

    p1 = p2;
    
    EXPECT_EQ(p2.ref_count(), 2_sz);

    auto p3{ p2 };
    
    EXPECT_EQ(p2.ref_count(), 3_sz);

    auto p4{ std::move(p2) };

    EXPECT_EQ(p4.ref_count(), 3_sz);

    auto p5{ py_object{} };
    
    p5 = std::move(p4);
    
    EXPECT_EQ(p5.ref_count(), 3_sz);
}