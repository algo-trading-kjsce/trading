/**
 * @file enums.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-17
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

namespace ta_helper
{

/**
 * @brief A list of strategies available from TA-Lib
 *
 */
enum ta_strategy
{
    TA_2CROWS,
    TA_3BLACKCROWS,
    TA_3INSIDE,
    TA_3LINESTRIKE,
    TA_3OUTSIDE,
    TA_3STARSINSOUTH,
    TA_3WHITESOLDIERS,
    TA_ADVANCEBLOCK,
    TA_BELTHOLD,
    TA_BREAKAWAY,
    TA_CLOSINGMARUBOZU,
    TA_CONCEALBABYSWALL,
    TA_COUNTERATTACK,
    TA_DOJI,
    TA_DOJISTAR,
    TA_DRAGONFLYDOJI,
    TA_ENGULFING,
    TA_GAPSIDESIDEWHITE,
    TA_GRAVESTONEDOJI,
    TA_HAMMER,
    TA_HANGINGMAN,
    TA_HARAMI,
    TA_HARAMICROSS,
    TA_HIGHWAVE,
    TA_HIKKAKE,
    TA_HIKKAKEMOD,
    TA_HOMINGPIGEON,
    TA_IDENTICAL3CROWS,
    TA_INNECK,
    TA_INVERTEDHAMMER,
    TA_KICKING,
    TA_KICKINGBYLENGTH,
    TA_LADDERBOTTOM,
    TA_LONGLEGGEDDOJI,
    TA_LONGLINE,
    TA_MARUBOZU,
    TA_MATCHINGLOW,
    TA_ONNECK,
    TA_PIERCING,
    TA_RICKSHAWMAN,
    TA_RISEFALL3METHODS,
    TA_SEPARATINGLINES,
    TA_SHOOTINGSTAR,
    TA_SHORTLINE,
    TA_SPINNINGTOP,
    TA_STALLEDPATTERN,
    TA_STICKSANDWICH,
    TA_TAKURI,
    TA_TASUKIGAP,
    TA_THRUSTING,
    TA_TRISTAR,
    TA_UNIQUE3RIVER,
    TA_UPSIDEGAP2CROWS,
    TA_XSIDEGAP3METHODS,

    // Unused strategies
    // TA_ABANDONEDBABY [[deprecated]],
    // TA_DARKCLOUDCOVER [[deprecated]],
    // TA_EVENINGDOJISTAR [[deprecated]],
    // TA_EVENINGSTAR [[deprecated]],
    // TA_MATHOLD [[deprecated]],
    // TA_MORNINGDOJISTAR [[deprecated]],
    // TA_MORNINGSTAR [[deprecated]],

    __LAST
};

}

/**
 * @brief Types of columns in csv files
 * 
 */
enum column_type : std::int32_t
{
    basic = 1 << 0,
    strategy = 1 << 1,
};


/**
 * @brief Enumeration for months in a year
 *
 */
enum class month
{
    jan = 1,
    feb = 2,
    mar = 3,
    apr = 4,
    may = 5,
    jun = 6,
    jul = 7,
    aug = 8,
    sep = 9,
    oct = 10,
    nov = 11,
    dec = 12,
};
