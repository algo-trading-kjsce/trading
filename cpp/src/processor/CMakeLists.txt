set(BINARY processor)

include_directories("${PROJECT_SOURCE_DIR}/include/processor/")

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

target_link_libraries(${BINARY} PUBLIC libta_lib.a PRIVATE Threads::Threads)
