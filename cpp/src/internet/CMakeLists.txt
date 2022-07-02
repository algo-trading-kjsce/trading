set(BINARY internet)

include_directories("${PROJECT_SOURCE_DIR}/include/${BINARY}/")

set(SOURCES ${SOURCES}
            curl_helper.cpp
            messenger.cpp
            stock_exchange.cpp)

add_library(${BINARY} STATIC ${SOURCES})

target_link_libraries(${BINARY} PUBLIC core CURL::libcurl)
