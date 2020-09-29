set(BINARY trading)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC processor)
