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

#include <algorithm>
#include <iostream>

#include "ta-lib/ta_func.h"
#include "timer.hpp"

#include "ta_utils.hpp"
#include "utilities.hpp"

namespace trading::ta_utilities
{
std::vector<std::int32_t> find_patterns( csv_data& io_csv_data )
{
    auto tmr{ timer{} };

    auto occurrences{ std::vector<std::int32_t>( 54, 0 ) };

    for( auto&& date : io_csv_data.dates )
    {
        auto raw_values{ io_csv_data.stock_map.at( date ).raw_values() };

        auto begin_idx{ 0 };
        auto n_elems{ 0 };

        auto number_of_entries{ raw_values.closes.size() };

        auto idx{ static_cast<ta_helper::ta_strategy>( 0 ) };

        auto HELPER = [&]( auto&& func ) mutable {
            auto patterns{ std::vector<std::int32_t>( number_of_entries, 0 ) };

            [[maybe_unused]] auto ret_code = func( 0,
                                                   static_cast<std::int32_t>( number_of_entries ) - 1,
                                                   raw_values.opens.data(),
                                                   raw_values.highs.data(),
                                                   raw_values.lows.data(),
                                                   raw_values.closes.data(),
                                                   std::addressof( begin_idx ),
                                                   std::addressof( n_elems ),
                                                   patterns.data() );

            occurrences.at( idx ) += static_cast<std::int32_t>(
                std::count_if( patterns.begin(), patterns.end(), []( auto&& pattern ) { return pattern != 0; } ) );

            io_csv_data.stock_map.at( date ).add_strategy_column( idx, std::move( patterns ) );

            idx = static_cast<ta_helper::ta_strategy>( idx + 1 );
        };

        HELPER( TA_CDL2CROWS );
        HELPER( TA_CDL3BLACKCROWS );
        HELPER( TA_CDL3INSIDE );
        HELPER( TA_CDL3LINESTRIKE );
        HELPER( TA_CDL3OUTSIDE );
        HELPER( TA_CDL3STARSINSOUTH );
        HELPER( TA_CDL3WHITESOLDIERS );
        HELPER( TA_CDLADVANCEBLOCK );
        HELPER( TA_CDLBELTHOLD );
        HELPER( TA_CDLBREAKAWAY );
        HELPER( TA_CDLCLOSINGMARUBOZU );
        HELPER( TA_CDLCONCEALBABYSWALL );
        HELPER( TA_CDLCOUNTERATTACK );
        HELPER( TA_CDLDOJI );
        HELPER( TA_CDLDOJISTAR );
        HELPER( TA_CDLDRAGONFLYDOJI );
        HELPER( TA_CDLENGULFING );
        HELPER( TA_CDLGAPSIDESIDEWHITE );
        HELPER( TA_CDLGRAVESTONEDOJI );
        HELPER( TA_CDLHAMMER );
        HELPER( TA_CDLHANGINGMAN );
        HELPER( TA_CDLHARAMI );
        HELPER( TA_CDLHARAMICROSS );
        HELPER( TA_CDLHIGHWAVE );
        HELPER( TA_CDLHIKKAKE );
        HELPER( TA_CDLHIKKAKEMOD );
        HELPER( TA_CDLHOMINGPIGEON );
        HELPER( TA_CDLIDENTICAL3CROWS );
        HELPER( TA_CDLINNECK );
        HELPER( TA_CDLINVERTEDHAMMER );
        HELPER( TA_CDLKICKING );
        HELPER( TA_CDLKICKINGBYLENGTH );
        HELPER( TA_CDLLADDERBOTTOM );
        HELPER( TA_CDLLONGLEGGEDDOJI );
        HELPER( TA_CDLLONGLINE );
        HELPER( TA_CDLMARUBOZU );
        HELPER( TA_CDLMATCHINGLOW );
        HELPER( TA_CDLONNECK );
        HELPER( TA_CDLPIERCING );
        HELPER( TA_CDLRICKSHAWMAN );
        HELPER( TA_CDLRISEFALL3METHODS );
        HELPER( TA_CDLSEPARATINGLINES );
        HELPER( TA_CDLSHOOTINGSTAR );
        HELPER( TA_CDLSHORTLINE );
        HELPER( TA_CDLSPINNINGTOP );
        HELPER( TA_CDLSTALLEDPATTERN );
        HELPER( TA_CDLSTICKSANDWICH );
        HELPER( TA_CDLTAKURI );
        HELPER( TA_CDLTASUKIGAP );
        HELPER( TA_CDLTHRUSTING );
        HELPER( TA_CDLTRISTAR );
        HELPER( TA_CDLUNIQUE3RIVER );
        HELPER( TA_CDLUPSIDEGAP2CROWS );
        HELPER( TA_CDLXSIDEGAP3METHODS );
        // HELPER(TA_CDLABANDONEDBABY);
        // HELPER(TA_CDLDARKCLOUDCOVER);
        // HELPER(TA_CDLEVENINGDOJISTAR);
        // HELPER(TA_CDLEVENINGSTAR);
        // HELPER(TA_CDLMATHOLD);
        // HELPER(TA_CDLMORNINGDOJISTAR);
        // HELPER(TA_CDLMORNINGSTAR);
    }

    {
        auto m{ trading::utilities::io_lock{} };
        std::cout << "File " << io_csv_data.m_filepath.filename().string() << " : Patterns found in "
                  << tmr.total_time().count() << "ms" << std::endl;
    }

    return occurrences;
}

}
