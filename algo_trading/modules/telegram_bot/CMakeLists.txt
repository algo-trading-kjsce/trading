set(BINARY telegram_bot)

set(SOURCES ${SOURCES}
            telegram_bot.cpp
)

set(HEADERS ${HEADERS}
            telegram_bot.hpp
)

add_library(${BINARY} STATIC ${SOURCES} ${HEADERS})

target_link_libraries(${BINARY} PUBLIC curl_helper)
