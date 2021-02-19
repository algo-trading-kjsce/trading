set(BINARY manager)

include_directories("${PROJECT_SOURCE_DIR}/include/${BINARY}/")

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

target_link_libraries(${BINARY} PUBLIC processor helper CURL::libcurl)