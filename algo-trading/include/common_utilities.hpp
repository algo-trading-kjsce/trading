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
    "",
    "datetime",
    "open",
    "high",
    "low",
    "close",
    "volume"
};


constexpr auto strategy_names = std::array
{
    "2CROWS",
    "3BLACKCROWS",
    "3INSIDE",
    "3LINESTRIKE",
    "3OUTSIDE",
    "3STARSINSOUTH",
    "3WHITESOLDIERS",
    "ADVANCEBLOCK",
    "BELTHOLD",
    "BREAKAWAY",
    "CLOSINGMARUBOZU",
    "CONCEALBABYSWALL",
    "COUNTERATTACK",
    "DOJI",
    "DOJISTAR",
    "DRAGONFLYDOJI",
    "ENGULFING",
    "GAPSIDESIDEWHITE",
    "GRAVESTONEDOJI",
    "HAMMER",
    "HANGINGMAN",
    "HARAMI",
    "HARAMICROSS",
    "HIGHWAVE",
    "HIKKAKE",
    "HIKKAKEMOD",
    "HOMINGPIGEON",
    "IDENTICAL3CROWS",
    "INNECK",
    "INVERTEDHAMMER",
    "KICKING",
    "KICKINGBYLENGTH",
    "LADDERBOTTOM",
    "LONGLEGGEDDOJI",
    "LONGLINE",
    "MARUBOZU",
    "MATCHINGLOW",
    "ONNECK",
    "PIERCING",
    "RICKSHAWMAN",
    "RISEFALL3METHODS",
    "SEPARATINGLINES",
    "SHOOTINGSTAR",
    "SHORTLINE",
    "SPINNINGTOP",
    "STALLEDPATTERN",
    "STICKSANDWICH",
    "TAKURI",
    "TASUKIGAP",
    "THRUSTING",
    "TRISTAR",
    "UNIQUE3RIVER",
    "UPSIDEGAP2CROWS",
    "XSIDEGAP3METHODS",

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
 * @param i_column_filter bitwise column filter
 *
 * @return column names in csv format
 */
template <auto filter>
auto get_column_name() noexcept
{
    auto ss{ std::stringstream{} };

    if constexpr (filter & column_type::basic)
    {
        for (auto&& basic_column : column_names)
        {
            ss << basic_column << delimiter;
        }
    }

    if constexpr (filter & column_type::strategy)
    {
        for (auto&& strategy : strategy_names)
        {
            ss << strategy << delimiter;
        }
    }
    return ss.str();
}

}
