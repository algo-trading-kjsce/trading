/**
 * @file test_macros.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-09-22
 *
 * @copyright Copyright (c) 2020
 *
 */


#define TEST_DATA_DIRECTORY std::filesystem::path{__FILE__}.parent_path().parent_path().append("data")

#define TEST_OUTPUT_DIRECTORY std::filesystem::path{__FILE__}.parent_path().parent_path().append("output")

#define TEST_NAME ::testing::UnitTest::GetInstance()->current_test_info()->name()

#define TEST_NAME_DATA TEST_DATA_DIRECTORY.append(TEST_NAME)