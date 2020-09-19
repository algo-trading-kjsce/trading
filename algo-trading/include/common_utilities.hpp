/**
 * @file ta_helper.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-16
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <array>
#include <sstream>
#include <string_view>

using namespace std::string_view_literals;

#include "includes.hpp"
#include "enums.hpp"

namespace utilities
{

/**
 * @brief An array of standard columns
 *
 */
constexpr auto column_names = std::array
{
    ""sv,
    "datetime"sv,
    "open"sv,
    "high"sv,
    "low"sv,
    "close"sv,
    "volume"sv
};


constexpr auto strategy_names = std::array
{
    "2CROWS"sv,
    "3BLACKCROWS"sv,
    "3INSIDE"sv,
    "3LINESTRIKE"sv,
    "3OUTSIDE"sv,
    "3STARSINSOUTH"sv,
    "3WHITESOLDIERS"sv,
    "ADVANCEBLOCK"sv,
    "BELTHOLD"sv,
    "BREAKAWAY"sv,
    "CLOSINGMARUBOZU"sv,
    "CONCEALBABYSWALL"sv,
    "COUNTERATTACK"sv,
    "DOJI"sv,
    "DOJISTAR"sv,
    "DRAGONFLYDOJI"sv,
    "ENGULFING"sv,
    "GAPSIDESIDEWHITE"sv,
    "GRAVESTONEDOJI"sv,
    "HAMMER"sv,
    "HANGINGMAN"sv,
    "HARAMI"sv,
    "HARAMICROSS"sv,
    "HIGHWAVE"sv,
    "HIKKAKE"sv,
    "HIKKAKEMOD"sv,
    "HOMINGPIGEON"sv,
    "IDENTICAL3CROWS"sv,
    "INNECK"sv,
    "INVERTEDHAMMER"sv,
    "KICKING"sv,
    "KICKINGBYLENGTH"sv,
    "LADDERBOTTOM"sv,
    "LONGLEGGEDDOJI"sv,
    "LONGLINE"sv,
    "MARUBOZU"sv,
    "MATCHINGLOW"sv,
    "ONNECK"sv,
    "PIERCING"sv,
    "RICKSHAWMAN"sv,
    "RISEFALL3METHODS"sv,
    "SEPARATINGLINES"sv,
    "SHOOTINGSTAR"sv,
    "SHORTLINE"sv,
    "SPINNINGTOP"sv,
    "STALLEDPATTERN"sv,
    "STICKSANDWICH"sv,
    "TAKURI"sv,
    "TASUKIGAP"sv,
    "THRUSTING"sv,
    "TRISTAR"sv,
    "UNIQUE3RIVER"sv,
    "UPSIDEGAP2CROWS"sv,
    "XSIDEGAP3METHODS"sv,

    // Unused strategies
    // "ABANDONEDBABY",
    // "DARKCLOUDCOVER",
    // "EVENINGDOJISTAR",
    // "EVENINGSTAR",
    // "MATHOLD",
    // "MORNINGDOJISTAR",
    // "MORNINGSTAR",
};


static_assert(strategy_names.size() == static_cast<std::size_t>(ta_helper::ta_strategy::__LAST), "you either added a strategy enum without its column name or vice versa!!");


/**
 * @brief Get the column names
 *
 * @tparam i_column_filter bitwise column filter
 *
 * @return column names in csv format
 */
template <auto filter>
auto get_column_name() noexcept
{
   auto ss{ std::stringstream{} };

   if constexpr ((filter & column_type::basic) > 0)
   {
      for (auto&& basic_column : column_names)
      {
         ss << basic_column << delimiter;
      }
   }

   if constexpr ((filter & column_type::strategy) > 0)
   {
      for (auto&& strategy : strategy_names)
      {
         ss << strategy << delimiter;
      }
   }
   return ss.str();
}


template <auto __filter>
constexpr auto char_count()
{
   auto count{ 0 };

   if constexpr ((__filter & column_type::basic) > 0)
   {
      for (auto&& name : utilities::column_names)
      {
         count += (name.size() + 1);
      }
   }

   if constexpr ((__filter & column_type::strategy) > 0)
   {
      for (auto&& name : utilities::strategy_names)
      {
         count += (name.size() + 1);
      }
   }

   return count;
}


template <auto filter>
constexpr auto get_column_names()
{
   auto arr{ std::array<char, char_count<filter>()>{} };

   auto idx{ 0 };

   if constexpr ((filter & column_type::basic) > 0)
   {
      for (auto&& name : utilities::column_names)
      {
         for (auto&& ch : name)
         {
            arr[idx++] = ch;
         }

         arr[idx++] = delimiter;
      }
   }

   if constexpr ((filter & column_type::strategy) > 0)
   {
      for (auto&& name : utilities::strategy_names)
      {
         for (auto&& ch : name)
         {
            arr[idx++] = ch;
         }

         arr[idx++] = delimiter;
      }
   }

   arr.back() = '\0';

   return arr;
}


constexpr auto basic_columns_arr{ get_column_names<column_type::basic>() };
constexpr auto basic_columns_str{ std::string_view{basic_columns_arr.data()} };

constexpr auto strategy_columns_arr{ get_column_names<column_type::strategy>() };
constexpr auto strategy_columns_str{ std::string_view{strategy_columns_arr.data()} };

constexpr auto all_columns_arr{ get_column_names<column_type::basic | column_type::strategy>() };
constexpr auto all_columns_str{ std::string_view{all_columns_arr.data()} };

}
