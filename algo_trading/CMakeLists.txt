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

## Common Libraries
# Core
add_subdirectory(libs/core)
# CURL
add_subdirectory(libs/curl)

## Node Libraries
# Translator
add_subdirectory(modules/translator)
# Messenger
add_subdirectory(modules/messenger)
# Planner
add_subdirectory(modules/planner)

## Nodes
# Translator
add_subdirectory(nodes/translator)
# Messenger
add_subdirectory(nodes/messenger)
# Planner
# add_subdirectory(nodes/planner)
# Trader
# add_subdirectory(nodes/trader)

## Tests
# add_subdirectory(libs/core/test)

link_directories("${PROJECT_SOURCE_DIR}/bin/")
