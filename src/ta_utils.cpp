/**
 * @file ta_utils.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <ta-lib/ta_func.h>

#include "timer.hpp"
#include "ta_utils.hpp"

#include <iostream>
namespace ta_utilities
{

ta_handler::ta_handler() noexcept
{
    [[maybe_unused]] auto retCode{ TA_Initialize() };
}


ta_handler::~ta_handler()
{
    TA_Shutdown();
}


std::vector<int> find_patterns([[maybe_unused]] csv_data_s& io_csv_data)
{
    auto tmr{ timer{} };

    io_csv_data.add_custom_column_name("20 SMA");

    io_csv_data.add_strategy_column_name("CDL2CROWS");
    io_csv_data.add_strategy_column_name("CDL3BLACKCROWS");
    io_csv_data.add_strategy_column_name("CDL3INSIDE");
    io_csv_data.add_strategy_column_name("CDL3LINESTRIKE");
    io_csv_data.add_strategy_column_name("CDL3OUTSIDE");
    io_csv_data.add_strategy_column_name("CDL3STARSINSOUTH");
    io_csv_data.add_strategy_column_name("CDL3WHITESOLDIERS");
    // io_csv_data.add_strategy_column_name("CDLABANDONEDBABY");
    io_csv_data.add_strategy_column_name("CDLADVANCEBLOCK");
    io_csv_data.add_strategy_column_name("CDLBELTHOLD");
    io_csv_data.add_strategy_column_name("CDLBREAKAWAY");
    io_csv_data.add_strategy_column_name("CDLCLOSINGMARUBOZU");
    io_csv_data.add_strategy_column_name("CDLCONCEALBABYSWALL");
    io_csv_data.add_strategy_column_name("CDLCOUNTERATTACK");
    // io_csv_data.add_strategy_column_name("CDLDARKCLOUDCOVER");
    io_csv_data.add_strategy_column_name("CDLDOJI");
    io_csv_data.add_strategy_column_name("CDLDOJISTAR");
    io_csv_data.add_strategy_column_name("CDLDRAGONFLYDOJI");
    io_csv_data.add_strategy_column_name("CDLENGULFING");
    // io_csv_data.add_strategy_column_name("CDLEVENINGDOJISTAR");
    // io_csv_data.add_strategy_column_name("CDLEVENINGSTAR");
    io_csv_data.add_strategy_column_name("CDLGAPSIDESIDEWHITE");
    io_csv_data.add_strategy_column_name("CDLGRAVESTONEDOJI");
    io_csv_data.add_strategy_column_name("CDLHAMMER");
    io_csv_data.add_strategy_column_name("CDLHANGINGMAN");
    io_csv_data.add_strategy_column_name("CDLHARAMI");
    io_csv_data.add_strategy_column_name("CDLHARAMICROSS");
    io_csv_data.add_strategy_column_name("CDLHIGHWAVE");
    io_csv_data.add_strategy_column_name("CDLHIKKAKE");
    io_csv_data.add_strategy_column_name("CDLHIKKAKEMOD");
    io_csv_data.add_strategy_column_name("CDLHOMINGPIGEON");
    io_csv_data.add_strategy_column_name("CDLIDENTICAL3CROWS");
    io_csv_data.add_strategy_column_name("CDLINNECK");
    io_csv_data.add_strategy_column_name("CDLINVERTEDHAMMER");
    io_csv_data.add_strategy_column_name("CDLKICKING");
    io_csv_data.add_strategy_column_name("CDLKICKINGBYLENGTH");
    io_csv_data.add_strategy_column_name("CDLLADDERBOTTOM");
    io_csv_data.add_strategy_column_name("CDLLONGLEGGEDDOJI");
    io_csv_data.add_strategy_column_name("CDLLONGLINE");
    io_csv_data.add_strategy_column_name("CDLMARUBOZU");
    io_csv_data.add_strategy_column_name("CDLMATCHINGLOW");
    // io_csv_data.add_strategy_column_name("CDLMATHOLD");
    // io_csv_data.add_strategy_column_name("CDLMORNINGDOJISTAR");
    // io_csv_data.add_strategy_column_name("CDLMORNINGSTAR");
    io_csv_data.add_strategy_column_name("CDLONNECK");
    io_csv_data.add_strategy_column_name("CDLPIERCING");
    io_csv_data.add_strategy_column_name("CDLRICKSHAWMAN");
    io_csv_data.add_strategy_column_name("CDLRISEFALL3METHODS");
    io_csv_data.add_strategy_column_name("CDLSEPARATINGLINES");
    io_csv_data.add_strategy_column_name("CDLSHOOTINGSTAR");
    io_csv_data.add_strategy_column_name("CDLSHORTLINE");
    io_csv_data.add_strategy_column_name("CDLSPINNINGTOP");
    io_csv_data.add_strategy_column_name("CDLSTALLEDPATTERN");
    io_csv_data.add_strategy_column_name("CDLSTICKSANDWICH");
    io_csv_data.add_strategy_column_name("CDLTAKURI");
    io_csv_data.add_strategy_column_name("CDLTASUKIGAP");
    io_csv_data.add_strategy_column_name("CDLTHRUSTING");
    io_csv_data.add_strategy_column_name("CDLTRISTAR");
    io_csv_data.add_strategy_column_name("CDLUNIQUE3RIVER");
    io_csv_data.add_strategy_column_name("CDLUPSIDEGAP2CROWS");
    io_csv_data.add_strategy_column_name("CDLXSIDEGAP3METHODS");

    auto occurrences{ std::vector<int>(54, 0) };

    for (auto&& date : io_csv_data.dates)
    {
        auto raw_values{ io_csv_data.stock_map.at(date).raw_values() };

        auto n_days{ 20 };

        auto begin_idx{ 0 };
        auto n_elems{ 0 };

        auto number_of_entries{ raw_values.closes.size() };

        {
            auto values{ std::vector<double>(number_of_entries, 0.0) };

            // Moving average
            [[maybe_unused]] auto ret_code{ TA_MA(
                0,
                static_cast<int>(number_of_entries) - 1,
                raw_values.closes.data(),
                n_days,
                TA_MAType::TA_MAType_SMA,
                std::addressof(begin_idx),
                std::addressof(n_elems),
                values.data() + n_days - 1) };

            io_csv_data.stock_map.at(date).add_custom_column(std::move(values));
        }

        auto idx{ static_cast<uint64_t>(0) };

        auto HELPER = [&](auto&& func) mutable
        {
            auto patterns{ std::vector<int>(number_of_entries, 0) };

            [[maybe_unused]] auto ret_code = func(
                0,
                static_cast<int>(number_of_entries) - 1,
                raw_values.opens.data(),
                raw_values.highs.data(),
                raw_values.lows.data(),
                raw_values.closes.data(),
                std::addressof(begin_idx),
                std::addressof(n_elems),
                patterns.data());

            occurrences.at(idx++) += std::count_if(patterns.begin(), patterns.end(), [](auto&& pattern)
            {
                return pattern != 0;
            });

            io_csv_data.stock_map.at(date).add_custom_column(std::vector<double>{patterns.begin(), patterns.end()});
        };

        HELPER(TA_CDL2CROWS);
        HELPER(TA_CDL3BLACKCROWS);
        HELPER(TA_CDL3INSIDE);
        HELPER(TA_CDL3LINESTRIKE);
        HELPER(TA_CDL3OUTSIDE);
        HELPER(TA_CDL3STARSINSOUTH);
        HELPER(TA_CDL3WHITESOLDIERS);
        //HELPER(TA_CDLABANDONEDBABY);
        HELPER(TA_CDLADVANCEBLOCK);
        HELPER(TA_CDLBELTHOLD);
        HELPER(TA_CDLBREAKAWAY);
        HELPER(TA_CDLCLOSINGMARUBOZU);
        HELPER(TA_CDLCONCEALBABYSWALL);
        HELPER(TA_CDLCOUNTERATTACK);
        //HELPER(TA_CDLDARKCLOUDCOVER);
        HELPER(TA_CDLDOJI);
        HELPER(TA_CDLDOJISTAR);
        HELPER(TA_CDLDRAGONFLYDOJI);
        HELPER(TA_CDLENGULFING);
        //HELPER(TA_CDLEVENINGDOJISTAR);
        //HELPER(TA_CDLEVENINGSTAR);
        HELPER(TA_CDLGAPSIDESIDEWHITE);
        HELPER(TA_CDLGRAVESTONEDOJI);
        HELPER(TA_CDLHAMMER);
        HELPER(TA_CDLHANGINGMAN);
        HELPER(TA_CDLHARAMI);
        HELPER(TA_CDLHARAMICROSS);
        HELPER(TA_CDLHIGHWAVE);
        HELPER(TA_CDLHIKKAKE);
        HELPER(TA_CDLHIKKAKEMOD);
        HELPER(TA_CDLHOMINGPIGEON);
        HELPER(TA_CDLIDENTICAL3CROWS);
        HELPER(TA_CDLINNECK);
        HELPER(TA_CDLINVERTEDHAMMER);
        HELPER(TA_CDLKICKING);
        HELPER(TA_CDLKICKINGBYLENGTH);
        HELPER(TA_CDLLADDERBOTTOM);
        HELPER(TA_CDLLONGLEGGEDDOJI);
        HELPER(TA_CDLLONGLINE);
        HELPER(TA_CDLMARUBOZU);
        HELPER(TA_CDLMATCHINGLOW);
        //HELPER(TA_CDLMATHOLD);
        //HELPER(TA_CDLMORNINGDOJISTAR);
        //HELPER(TA_CDLMORNINGSTAR);
        HELPER(TA_CDLONNECK);
        HELPER(TA_CDLPIERCING);
        HELPER(TA_CDLRICKSHAWMAN);
        HELPER(TA_CDLRISEFALL3METHODS);
        HELPER(TA_CDLSEPARATINGLINES);
        HELPER(TA_CDLSHOOTINGSTAR);
        HELPER(TA_CDLSHORTLINE);
        HELPER(TA_CDLSPINNINGTOP);
        HELPER(TA_CDLSTALLEDPATTERN);
        HELPER(TA_CDLSTICKSANDWICH);
        HELPER(TA_CDLTAKURI);
        HELPER(TA_CDLTASUKIGAP);
        HELPER(TA_CDLTHRUSTING);
        HELPER(TA_CDLTRISTAR);
        HELPER(TA_CDLUNIQUE3RIVER);
        HELPER(TA_CDLUPSIDEGAP2CROWS);
        HELPER(TA_CDLXSIDEGAP3METHODS);
    }

    std::cout << "Patterns found in " << tmr.total_time().count() << "ms\n";

    return occurrences;
}

}
