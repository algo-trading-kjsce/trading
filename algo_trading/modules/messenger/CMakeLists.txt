set(BINARY messenger_impl)

set(SOURCES ${SOURCES}
            messenger.cpp
)

set(HEADERS ${HEADERS}
            messenger.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC curl_helper)
