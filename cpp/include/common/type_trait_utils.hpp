/**
 * @file type_trait_utils.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <cstdint>
#include <type_traits>

#define REQUIRES( ... ) typename = std::enable_if_t<__VA_ARGS__>

#define UNSIGNED_CONVERTER( x ) \
    constexpr auto operator""_ui##x( unsigned long long int __n ) noexcept \
    { \
        return static_cast<uint##x##_t>( __n ); \
    }
#define SIGNED_CONVERTER( x ) \
    constexpr auto operator""_i##x( unsigned long long int __n ) noexcept \
    { \
        return static_cast<int##x##_t>( __n ); \
    }

#define CONVERTER( x ) UNSIGNED_CONVERTER( x ) SIGNED_CONVERTER( x )

CONVERTER( 8 );
CONVERTER( 16 );
CONVERTER( 32 );
CONVERTER( 64 );

constexpr auto operator""_sz( unsigned long long int __n ) noexcept
{
    return static_cast<std::size_t>( __n );
}

template<typename _T, REQUIRES( !std::is_pointer_v<_T> )>
auto get_char_address( _T&& object )
{
    return (char*)std::addressof( object );
}


#define io_info( x ) get_char_address( x ), sizeof( x )
