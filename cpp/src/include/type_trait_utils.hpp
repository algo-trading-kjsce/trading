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

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

#define CONVERT_(x, ...) constexpr auto operator""_##__VA_ARGS__##i##x(unsigned long long int __n) noexcept { return static_cast<##__VA_ARGS__##int##x##_t>(__n); }

#define CONVERTER(x) CONVERT_(x, u) CONVERT_(x)

CONVERTER(8);
CONVERTER(16);
CONVERTER(32);
CONVERTER(64);

constexpr auto operator""_sz(unsigned long long int __n) noexcept
{
    return static_cast<std::size_t>(__n);
}

template <typename _T, REQUIRES(!std::is_pointer_v<_T>)>
auto get_char_address(_T&& object)
{
    return (char*) std::addressof(object);
}


#define io_info(x) get_char_address(x), sizeof(x)
