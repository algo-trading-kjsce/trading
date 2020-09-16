cmake_minimum_required(VERSION 3.10)

project(algo_trading)

set(CMAKE_CXX_STANDARD 20)


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS -m32)
set(CMAKE_CXX_FLAGS -m32)


# Set platform values
if (CMAKE_HOST_WIN32)
  set(WINDOWS 1)
elseif(CMAKE_HOST_APPLE)
  set(MACOS 1)
elseif(CMAKE_HOST_UNIX)
  set(LINUX 1)
endif()

include_directories("${PROJECT_SOURCE_DIR}/include/")

# set warning flags
if(NOT WINDOWS)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif()

add_subdirectory(pattern)
