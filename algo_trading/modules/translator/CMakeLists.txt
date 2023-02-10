set(BINARY translator_impl)

set(SOURCES ${SOURCES}
            translator.cpp
)

set(HEADERS ${HEADERS}
            translator.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC core)
