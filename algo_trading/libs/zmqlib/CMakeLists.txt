set(BINARY zmqlib)

set(SOURCES ${SOURCES}
            utils.cpp)

set(HEADERS ${HEADERS}
            channel_type.hpp
            create_socket.hpp
            utils.hpp)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC cppzmq)
