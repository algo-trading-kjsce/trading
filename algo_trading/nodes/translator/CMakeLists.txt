set(BINARY translator)

set(SOURCES ${SOURCES}
            translator.cpp)

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} PUBLIC translator_impl core cppzmq)
