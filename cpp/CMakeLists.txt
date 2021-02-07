cmake_minimum_required(VERSION 3.10)

project(algo-trading)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)
find_package(PythonLibs REQUIRED)

include(gtest.cmake)

include_directories("${PYTHON_INCLUDE_DIRS}")
include_directories("${PROJECT_SOURCE_DIR}/include/")
include_directories("${PROJECT_SOURCE_DIR}/include/common/")

if(UNIX)
  link_directories("${PROJECT_SOURCE_DIR}/lib/unix/")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# source projects
add_subdirectory(src/pylib)
add_subdirectory(src/processor)
add_subdirectory(src/trading)

# tests
add_subdirectory(tst/tests)
