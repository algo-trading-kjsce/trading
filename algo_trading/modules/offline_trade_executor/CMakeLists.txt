set(BINARY offline_trader)

set(SOURCES ${SOURCES}
            trade_executor.cpp
)

set(HEADERS ${HEADERS}
            trade_executor.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC curl)