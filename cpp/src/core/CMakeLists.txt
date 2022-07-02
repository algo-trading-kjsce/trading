set(BINARY core)

include_directories("${PROJECT_SOURCE_DIR}/include/${BINARY}/")

set(SOURCES ${SOURCES}
            candle.cpp
            date.cpp
            json_helper.cpp
            stock_data.cpp
            time.cpp
            timer.cpp)

add_library(${BINARY} STATIC ${SOURCES})
