set(BINARY strategy)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC libta_lib.a)

# add_test(NAME ${BINARY} COMMAND ${BINARY})

# target_link_libraries(${BINARY} PUBLIC gtest)

# add_custom_command(TARGET ${BINARY} POST_BUILD COMMAND ${BINARY})
