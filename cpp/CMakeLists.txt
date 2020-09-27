cmake_minimum_required(VERSION 3.10)

project(algo-trading)

set(CMAKE_CXX_STANDARD 17)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

include(gtest.cmake)

include_directories("${PROJECT_SOURCE_DIR}/src/include/")

if(UNIX)
  link_directories("${PROJECT_SOURCE_DIR}/lib/unix/")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

find_package(Threads)

# source projects
add_subdirectory(src/csv_utilities)
add_subdirectory(src/resolution)
add_subdirectory(src/trading)

# tests
add_subdirectory(tests/csv_utilities_tests)
add_subdirectory(tests/resolution_tests)
