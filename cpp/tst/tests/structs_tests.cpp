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

#include "type_trait_utils.hpp"

#include "structs.hpp"

TEST( structs_tests, date_equality_test )
{
    auto date1{ date_s{ 2015, month::aug, 12 } };

    auto date2{ date1 };

    EXPECT_EQ( date1, date2 );

    date2.m_month = month::apr;

    EXPECT_NE( date1, date2 );
}


TEST( structs_tests, date_str_test )
{
    auto date{ date_s{ 2012, month::feb, 10 } };

    EXPECT_EQ( date.to_str(), "2012-02-10" );
}


TEST( structs_tests, time_equality_test )
{
    auto time1{ time_s{ 12, 34, 34 } };

    auto time2{ time1 };

    EXPECT_EQ( time1, time2 );

    time2.minutes = 23;

    EXPECT_NE( time1, time2 );
}


TEST( structs_tests, time_str_test )
{
    auto time{ time_s{ 23, 45, 12 } };

    EXPECT_EQ( time.to_str(), "23:45:12" );
}


TEST( structs_tests, time_difference_test )
{
    auto time1{ time_s{ 17, 45, 56 } };

    auto time2{ time1 };

    EXPECT_EQ( time1 - time2, 0 );

    auto time3{ time_s{ 17, 23, 56 } };

    EXPECT_EQ( time1 - time3, 22 );
    EXPECT_EQ( time3 - time1, -22 );
}


TEST( structs_tests, time_add_test )
{
    auto time{ time_s{ 17, 23 } };

    auto time1{ time };
    time1.add_minutes( 23 );
    EXPECT_EQ( time1, ( time_s{ 17, 46 } ) );

    auto time2{ time };
    time2.add_minutes( 43 );
    EXPECT_EQ( time2, ( time_s{ 18, 6 } ) );

    auto time3{ time };
    time3.add_minutes( 103 );
    EXPECT_EQ( time3, ( time_s{ 19, 6 } ) );
}


TEST( structs_tests, create_date_test_success )
{
    auto o_date1{ date_s::try_create_date( "2039-12-22" ) };

    EXPECT_TRUE( o_date1.has_value() );

    EXPECT_EQ( o_date1.value(), ( date_s{ 2039_i32, month::dec, 22 } ) );
}


TEST( structs_tests, create_date_test_fail )
{
    auto o_date1{ date_s::try_create_date( "" ) };

    EXPECT_FALSE( o_date1.has_value() );

    o_date1 = date_s::try_create_date( "2022-12" );

    EXPECT_FALSE( o_date1.has_value() );
}


TEST( structs_tests, create_time_test_success )
{
    auto o_time1{ time_s::try_create_time( "12:23:12" ) };

    EXPECT_TRUE( o_time1.has_value() );

    EXPECT_EQ( o_time1.value(), ( time_s{ 12, 23, 12 } ) );

    o_time1 = time_s::try_create_time( "12:23:12+00:00" );

    EXPECT_TRUE( o_time1.has_value() );

    EXPECT_EQ( o_time1.value(), ( time_s{ 12, 23, 12 } ) );

    o_time1 = time_s::try_create_time( "12:23" );

    EXPECT_TRUE( o_time1.has_value() );

    EXPECT_EQ( o_time1.value(), ( time_s{ 12, 23 } ) );
}


TEST( structs_tests, create_time_test_fail )
{
    auto o_time1{ time_s::try_create_time( "" ) };

    EXPECT_FALSE( o_time1.has_value() );
}


TEST( structs_tests, create_candle_test_success )
{
    auto o_candle1{ candle_s::try_create_candle( "1001,2019-08-29 09:15:00+00:00,42.5,46.5,47.9,41.0,50200" ) };

    EXPECT_TRUE( o_candle1.has_value() );

    EXPECT_EQ( o_candle1->volume, 50200_ui32 );

    EXPECT_DOUBLE_EQ( o_candle1->low, 47.9 );

    auto o_candle2{ candle_s::try_create_candle( "1001,2019-08-29 09:15:00,42.5,46.5,47.9,41.0,50200" ) };

    EXPECT_EQ( o_candle1, o_candle2 );
}


TEST( structs_tests, create_candle_test_fail )
{
    auto o_candle1{ candle_s::try_create_candle( "1001,2019-08-29 09:15:00+00:00,42.5,46.5,47.9,41.0" ) };

    EXPECT_FALSE( o_candle1.has_value() );

    o_candle1 = candle_s::try_create_candle( "" );

    EXPECT_FALSE( o_candle1.has_value() );
}
