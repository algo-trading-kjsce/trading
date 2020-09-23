/**
 * @file structs_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-19
 *
 * @copyright Copyright (c) 2020
 *
 */


#include "gtest/gtest.h"

#include "structs.hpp"

TEST(structs_tests, date_equality_test)
{
    auto date1{ date_s{2015, month::aug, 12} };

    auto date2{ date1 };

    EXPECT_TRUE(date1 == date2);

    date2.m_month = month::apr;

    EXPECT_FALSE(date1 == date2);
}


TEST(structs_tests, date_str_test)
{
    auto date{ date_s{2012, month::feb, 10} };

    EXPECT_EQ(date.to_str(), "2012-02-10");
}


TEST(structs_tests, time_equality_test)
{
    auto time1{ time_s{12, 34, 34} };

    auto time2{ time1 };

    EXPECT_TRUE(time1 == time2);

    time2.minutes = 23;

    EXPECT_FALSE(time1 == time2);
}


TEST(structs_tests, time_str_test)
{
    auto time{ time_s{23, 45, 12} };

    EXPECT_EQ(time.to_str(), "23:45:12");
}


TEST(structs_tests, time_difference_test)
{
    auto time1{ time_s{17, 45, 56} };

    auto time2{ time1 };

    EXPECT_TRUE((time1 - time2) == 0);

    auto time3{ time_s{17, 23, 56} };

    EXPECT_TRUE((time1 - time3) == 22);
    
    EXPECT_TRUE((time3 - time1) == -22);
}


TEST(structs_tests, time_add_test)
{
    auto time{ time_s{17, 23} };
    
    auto time1{ time };
    time1.add_minutes(23);
    EXPECT_TRUE(time1 == (time_s{ 17, 46 }));

    auto time2{ time };
    time2.add_minutes(43);
    EXPECT_TRUE(time2 == (time_s{ 18, 6 }));

    auto time3{ time };
    time3.add_minutes(103);
    EXPECT_TRUE(time3 == (time_s{ 19, 6 }));
}
