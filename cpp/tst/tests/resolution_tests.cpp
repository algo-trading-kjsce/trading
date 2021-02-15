/**
 * @file resolution_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <filesystem>

#include "gtest/gtest.h"

#include "../test_macros.hpp"

#include "type_trait_utils.hpp"

#include "processor/utilities.hpp"
#include "processor/resolution.hpp"


namespace
{
auto change_resolution_test_code( const csv_data& i_csv_data,
                                  const std::filesystem::path& i_output_file_path,
                                  std::int32_t i_new_resolution )
{
    auto new_csv{ trading::csv::resolution::change_resolution( i_csv_data, i_new_resolution ) };

    trading::utilities::write_csv( new_csv, i_output_file_path, false );

    EXPECT_TRUE( std::filesystem::exists( i_output_file_path ) );

    auto test_csv_data{ trading::utilities::read_initial_csv( i_output_file_path ) };

    EXPECT_EQ( trading::csv::resolution::find_candle_size( test_csv_data ), i_new_resolution );
}

}


TEST( resolution_tests, candle_size_test )
{
    auto buffer_{ buffer_manager{} };

    auto directory_path{ TEST_DATA_DIRECTORY };

    auto csv_5min_file{ directory_path.append( "5min_data.csv" ) };

    auto csv_data{ trading::utilities::read_initial_csv( csv_5min_file ) };

    auto candle_size{ trading::csv::resolution::find_candle_size( csv_data ) };

    EXPECT_EQ( candle_size, 300_i32 );
}


TEST( resolution_tests, combine_candles_test )
{
    auto buffer_{ buffer_manager{} };

    auto date{ date_s{ 2019, month::apr, 12 } };

    auto candle1{ candle_s{ 0, 123, date, time_s{ 12, 05 }, 100.0, 125.0, 95.0, 110.0 } };
    auto candle2{ candle_s{ 1, 190, date, time_s{ 12, 10 }, 120.0, 120.0, 75.0, 117.0 } };
    auto candle3{ candle_s{ 2, 134, date, time_s{ 12, 15 }, 100.0, 125.0, 90.0, 80.0 } };

    auto candles{ std::list{ candle1, candle2, candle3 } };

    auto expected_candle{ candle_s{ 0, 149, date, time_s{ 12, 05 }, 100.0, 125.0, 75.0, 80.0 } };

    auto combined_3_candles{ trading::csv::resolution::combine_candles(
        date, candles.begin(), candles.end(), candles.size() ) };

    EXPECT_EQ( expected_candle, combined_3_candles );

    expected_candle = candle_s{ 0, 156, date, time_s{ 12, 05 }, 100.0, 125.0, 75.0, 117.0 };

    auto combined_2_candles{ trading::csv::resolution::combine_candles(
        date, candles.begin(), std::next( candles.begin(), 2 ), 2_sz ) };

    EXPECT_EQ( expected_candle, combined_2_candles );
}


TEST( resolution_tests, change_resolution_test_success )
{
    auto buffer_{ buffer_manager{} };

    auto directory_path{ TEST_DATA_DIRECTORY };

    auto csv_5min_file{ directory_path.append( "5min_data.csv" ) };

    auto csv_data{ trading::utilities::read_initial_csv( csv_5min_file ) };

    auto output_file{ TEST_OUTPUT_DIRECTORY.append( "5min_data.csv" ) };

    EXPECT_NO_THROW( change_resolution_test_code( csv_data, output_file, 900_i32 ) );
    EXPECT_NO_THROW( change_resolution_test_code( csv_data, output_file, 1200_i32 ) );
}


TEST( resolution_tests, change_resolution_test_failure )
{
    auto buffer_{ buffer_manager{} };

    auto directory_path{ TEST_DATA_DIRECTORY };

    auto csv_5min_file{ directory_path.append( "5min_data.csv" ) };

    auto csv_data{ trading::utilities::read_initial_csv( csv_5min_file ) };

    auto output_file{ TEST_OUTPUT_DIRECTORY.append( "5min_data.csv" ) };

    EXPECT_THROW( change_resolution_test_code( csv_data, output_file, 1260_i32 ), std::invalid_argument );
}
