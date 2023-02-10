set(BINARY core_tests)

set(SOURCES ${SOURCES}
            candle-test.cpp
            # node_info-test.cpp
            task-test.cpp
            trade_request-test.cpp)

enable_testing()

add_test(NAME ${BINARY} COMMAND ${BINARY})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC core GTest::GTest GTest::Main)

#add_custom_command(TARGET ${BINARY} POST_BUILD COMMAND ${BINARY})
