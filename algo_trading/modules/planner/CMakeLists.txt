set(BINARY planner_impl)

set(SOURCES ${SOURCES}
            planner.cpp
            trade_block.cpp
)

set(HEADERS ${HEADERS}
            planner.hpp
            trade_block.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC core)
