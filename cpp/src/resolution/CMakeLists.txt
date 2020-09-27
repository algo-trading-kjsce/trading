set(BINARY resolution)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})
