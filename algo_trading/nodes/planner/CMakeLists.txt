set(BINARY planner)

set(SOURCES ${SOURCES}
            planner.cpp)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC planner_impl converter core cppzmq)
