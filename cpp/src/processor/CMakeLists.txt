set(BINARY processor)

include_directories("${PROJECT_SOURCE_DIR}/include/${BINARY}/")

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

if (UNIX)
  target_link_libraries(${BINARY} PUBLIC libta_lib.a PRIVATE Threads::Threads)
elseif(WIN32)
  target_link_libraries(${BINARY} PUBLIC ta_libc_csr.lib PRIVATE Threads::Threads)
endif()
