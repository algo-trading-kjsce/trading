cmake_minimum_required(VERSION 3.10)

project(algo-trading)

set(CMAKE_CXX_STANDARD 20)


link_directories("${PROJECT_SOURCE_DIR}/lib/")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(Threads REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(CURL REQUIRED)

include(gtest.cmake)

include_directories("${PYTHON_INCLUDE_DIRS}")
include_directories("${PROJECT_SOURCE_DIR}/include/")
include_directories("${PROJECT_SOURCE_DIR}/include/common/")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/include/3rdparty/")

add_compile_definitions(PYTHON_FILES="${PROJECT_SOURCE_DIR}/../python/")

# source projects
add_subdirectory(src/helper)
add_subdirectory(src/processor)
add_subdirectory(src/manager)
add_subdirectory(src/trading)

# tests
add_subdirectory(tst/tests)
