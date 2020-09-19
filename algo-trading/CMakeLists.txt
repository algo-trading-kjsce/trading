cmake_minimum_required(VERSION 3.10)

project(algo-trading)

set(CMAKE_CXX_STANDARD 17)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(gtest.cmake)

include_directories("${PROJECT_SOURCE_DIR}/include/")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(Threads)

add_subdirectory(csv_utilities)
add_subdirectory(csv_utilities_tests)

add_subdirectory(pattern)
