set(BINARY task_translator)

set(SOURCES ${SOURCES}
            task_translator.cpp
)

set(HEADERS ${HEADERS}
            task_translator.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC fmt::fmt core)
