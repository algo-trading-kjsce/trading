set(BINARY translator)

set(SOURCES ${SOURCES}
            translator.cpp)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC zmqlib task_translator core cppzmq)
