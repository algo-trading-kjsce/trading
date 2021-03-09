cmake_minimum_required(VERSION 3.10)

project(algo-trading)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)

  set(PYTHON_INCLUDE_DIR "C:/Program Files (x86)/Python38-32/include/")
  set(PYTHON_LIBRARIES "C:/Program Files (x86)/Python38-32/libs/")

  set(CURL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/3rdparty/curl")
  set(CURL_LIBRARY "${PROJECT_SOURCE_DIR}/lib/win32/libcurl.lib")
  add_definitions(-DCURL_STATICLIB)

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O3")

  include(FindCURL)

  link_directories("${PROJECT_SOURCE_DIR}/lib/win32/" "${PYTHON_LIBRARIES}")

  include_directories(${CURL_INCLUDE_DIR})

elseif(UNIX)

  link_directories("${PROJECT_SOURCE_DIR}/lib/unix/")

  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")

endif()

find_package(Threads REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(PythonInterp REQUIRED)
find_package(CURL REQUIRED)

include(gtest.cmake)

include_directories("${PYTHON_INCLUDE_DIRS}")
include_directories("${PROJECT_SOURCE_DIR}/include/")
include_directories("${PROJECT_SOURCE_DIR}/include/common/")
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/include/3rdparty/")

set(CMAKE_FINAL_OUTPUT "${PROJECT_SOURCE_DIR}/../build/package/")
set(CMAKE_FILE_TRANSFER_SCRIPT "${PROJECT_SOURCE_DIR}/transfer_output.py")

add_compile_definitions(PYTHON_FILES="${CMAKE_FINAL_OUTPUT}")

# source projects
add_subdirectory(src/helper)
add_subdirectory(src/processor)
add_subdirectory(src/manager)
add_subdirectory(src/trading)

# tests
add_subdirectory(tst/tests)

# Post Build commands
execute_process(COMMAND "${PYTHON_EXECUTABLE}" "${CMAKE_FILE_TRANSFER_SCRIPT}" "copy" "${PROJECT_SOURCE_DIR}/../python/" "${CMAKE_FINAL_OUTPUT}" ".py")