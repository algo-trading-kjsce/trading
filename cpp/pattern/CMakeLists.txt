set(BINARY pattern)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

if(WINDOWS)
  link_directories("${PROJECT_SOURCE_DIR}/lib/")
endif()

add_executable(${BINARY} ${SOURCES})

if(WINDOWS)
  target_link_libraries(${BINARY} PUBLIC ta_libc_csd ta_func_csd ta_common_csd ta_abstract_csd)
elseif(LINUX)
  target_link_libraries(${BINARY} PUBLIC libta_lib.a)
endif()
