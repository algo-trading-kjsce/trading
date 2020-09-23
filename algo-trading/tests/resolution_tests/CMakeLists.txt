
set(BINARY resolution_tests)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC gtest ${CMAKE_THREAD_LIBS_INIT} resolution csv_utilities)

if(CMAKE_BUILD_TYPE MATCHES Release)
  add_custom_command(TARGET ${BINARY} POST_BUILD COMMAND ${BINARY})
endif()
