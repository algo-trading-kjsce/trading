cmake_minimum_required(VERSION 3.10)

project(algo_trading)

# C++ standard
set(CMAKE_CXX_STANDARD 20)

# compiler options
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic -fconcepts")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# dependencies
find_package(CURL REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(cppzmq REQUIRED)

# Include directories
include_directories("${PROJECT_SOURCE_DIR}/")
include_directories("${PROJECT_SOURCE_DIR}/include/")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/ext/")
include_directories(${GTEST_INCLUDE_DIRS})

# Set the minimum required version of Boost
set(Boost_MIN_VERSION "1.76.0")

# Find the Boost Program Options library
find_package(Boost ${Boost_MIN_VERSION} REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
    message("SUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU")
else()
    message(FATAL_ERROR "Boost Program Options library not found.")
endif()

## Common Libraries
# ZMQLib
add_subdirectory(libs/zmqlib)
# Core
add_subdirectory(libs/core)
# CURL
add_subdirectory(libs/curl)

## Node Libraries
# Translator
add_subdirectory(modules/task_translator)
# Messenger
add_subdirectory(modules/telegram_bot)
# Planner
add_subdirectory(modules/trade_planner)

## Nodes
# Translator
add_subdirectory(nodes/translator)
# Messenger
add_subdirectory(nodes/messenger)
# Planner
add_subdirectory(nodes/planner)
# Trader
# add_subdirectory(nodes/trader)
# Manager
add_subdirectory(nodes/manager)

## Tests
# add_subdirectory(libs/core/test)

link_directories("${PROJECT_SOURCE_DIR}/bin/")
