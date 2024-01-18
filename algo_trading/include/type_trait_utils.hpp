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

#include <concepts>
#include <cstdint>
#include <memory>
#include <sstream>
#include <type_traits>

#ifdef __cpp_consteval
#    define __CONSTEVAL consteval
#else
#    define __CONSTEVAL constexpr
#endif

#define REQUIRES( ... ) typename = std::enable_if_t<__VA_ARGS__>

#define UNSIGNED_CONVERTER( x ) \
    __CONSTEVAL auto operator""_ui##x( unsigned long long int __n ) noexcept \
    { \
        return static_cast<std::uint##x##_t>( __n ); \
    }
#define SIGNED_CONVERTER( x ) \
    __CONSTEVAL auto operator""_i##x( unsigned long long int __n ) noexcept \
    { \
        return static_cast<std::int##x##_t>( __n ); \
    }

#define CONVERTER( x ) UNSIGNED_CONVERTER( x ) SIGNED_CONVERTER( x )

CONVERTER( 8 )
CONVERTER( 16 )
CONVERTER( 32 )
CONVERTER( 64 )

__CONSTEVAL auto operator""_sz( unsigned long long int __n ) noexcept
{
    return static_cast<std::size_t>( __n );
}

template<typename _T, REQUIRES( !std::is_pointer_v<_T> )>
inline auto get_char_address( _T&& object )
{
    return (char*)std::addressof( object );
}

template<typename T, REQUIRES( std::is_enum_v<T> )>
inline std::string to_str( const T& i_obj )
{
    std::stringstream os;
    os << i_obj;
    return os.str();
}

#define io_info( x ) get_char_address( x ), sizeof( x )


#define no_copy_class( x ) \
    x( const x& ) = delete; \
    x& operator=( const x& ) = delete

#define no_move_class( x ) \
    x( x&& ) = delete; \
    x& operator=( x&& ) = delete

#define no_copy_no_move_class( x ) \
    no_copy_class( x ); \
    no_move_class( x )
