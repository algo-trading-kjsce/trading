set(BINARY curl_helper)

set(SOURCES ${SOURCES}
            curl_handler.cpp
            curl_helper.cpp
)

set(HEADERS ${HEADERS}
            curl_handler.hpp
            curl_helper.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC CURL::libcurl)
