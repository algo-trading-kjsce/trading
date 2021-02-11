/**
 * @file type_traits_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "gtest/gtest.h"

#include "type_trait_utils.hpp"


TEST( type_traits_tests, int_conversion_test )
{
    static_assert( std::is_same_v<std::int8_t, decltype( 34_i8 )> );
    static_assert( std::is_same_v<std::uint8_t, decltype( 34_ui8 )> );

    static_assert( std::is_same_v<std::int16_t, decltype( 34_i16 )> );
    static_assert( std::is_same_v<std::uint16_t, decltype( 34_ui16 )> );


    static_assert( std::is_same_v<std::int32_t, decltype( 34_i32 )> );
    static_assert( std::is_same_v<std::uint32_t, decltype( 34_ui32 )> );

    static_assert( std::is_same_v<std::int64_t, decltype( 34_i64 )> );
    static_assert( std::is_same_v<std::uint64_t, decltype( 34_ui64 )> );

    static_assert( std::is_same_v<std::size_t, decltype( 123_sz )> );
}


TEST( type_traits_tests, char_address_test )
{
    auto expected{ -4322_i32 };

    auto ptr{ get_char_address( expected ) };

    auto test{ *reinterpret_cast<decltype( expected )*>( ptr ) };

    EXPECT_EQ( test, expected );
}
