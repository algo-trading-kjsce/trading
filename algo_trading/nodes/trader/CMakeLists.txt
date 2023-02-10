set(BINARY trader)

set(SOURCES ${SOURCES}
            trader.cpp)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC converter core cppzmq)
