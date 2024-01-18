set(BINARY trade_planner)

set(SOURCES ${SOURCES}
            trade_planner.cpp
            trade_block.cpp
)

set(HEADERS ${HEADERS}
            trade_planner.hpp
            trade_block.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC core)
