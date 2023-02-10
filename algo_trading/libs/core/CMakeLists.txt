set(BINARY core)

set(SOURCES ${SOURCES}
            candle.cpp
            task.cpp
            trade_request.cpp
            utils.cpp
)

set(HEADERS ${HEADERS}
            candle.hpp
            channel_type.hpp
            node_info.hpp
            node_type.hpp
            task.hpp
            task_type.hpp
            trade_request.hpp
            trade_type.hpp
            utils.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC fmt::fmt cppzmq)
