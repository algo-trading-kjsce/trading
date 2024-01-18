set(BINARY manager)

set(SOURCES ${SOURCES}
            manager.cpp)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC zmqlib cppzmq)
