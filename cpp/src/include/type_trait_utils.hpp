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

#define UNSIGNED_CONVERTER(x) constexpr auto operator""_ui##x(unsigned long long int __n) noexcept { return static_cast<uint##x##_t>(__n); }
#define SIGNED_CONVERTER(x) constexpr auto operator""_i##x(unsigned long long int __n) noexcept { return static_cast<int##x##_t>(__n); }

#define CONVERTER(x) UNSIGNED_CONVERTER(x) SIGNED_CONVERTER(x)

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


namespace
{

template <std::size_t _sz, bool _unsigned>
constexpr auto integer_type() noexcept
{
#define FUNCTOR(i, j) if constexpr ((_sz) == (i)) { if constexpr (_unsigned) { return std::uint##j##_t{}; } else { return std::int##j##_t{}; } }

    FUNCTOR(1_sz, 8);
    FUNCTOR(2_sz, 16);
    FUNCTOR(4_sz, 32);
    FUNCTOR(8_sz, 64);
}

template <typename _T>
constexpr auto integer_type() noexcept
{
    if constexpr (std::is_enum_v<_T>)
    {
        using int_t = std::underlying_type_t<_T>;

        return integer_type<sizeof(int_t), std::is_unsigned_v<_T>>();
    }
    else
    {
        return integer_type<sizeof(_T), std::is_unsigned_v<_T>>();
    }
}

}


/**
 * @brief Helper template for retrieving 2 byte integers from string
 *
 * @tparam _T type of 2 byte integer
 * @param i_str Incoming string
 * @return 2 byte integer
 */
template <typename _T, REQUIRES(std::is_integral_v<_T> || std::is_enum_v<_T>)>
auto str_to(const std::string& i_str)
{
    auto i{ std::stoi(i_str) };

    if (i > static_cast<std::int32_t>(std::numeric_limits<_T>::max()))
    {
        throw std::out_of_range{ "Number out of range" };
    }

    return static_cast<_T>(i);
}
