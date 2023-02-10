set(BINARY messenger)

set(SOURCES ${SOURCES}
            messenger.cpp)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC messenger_impl core cppzmq)
