set(BINARY core)

set(SOURCES ${SOURCES}
            candle.cpp
            task.cpp
            ticker_info.cpp
            utils.cpp
)

set(HEADERS ${HEADERS}
            candle.hpp
            node_info.hpp
            node_type.hpp
            task.hpp
            task_type.hpp
            ticker_info.hpp
            utils.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC fmt::fmt)
