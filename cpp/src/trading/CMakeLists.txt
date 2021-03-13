set(BINARY trading)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC manager)

if(WIN32)
  add_custom_command(TARGET ${BINARY} POST_BUILD COMMAND "${PYTHON_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/post_build.py" "copy_files_of_type" "${CMAKE_CURRENT_BINARY_DIR}" ".exe" ".pdb")
endif()
