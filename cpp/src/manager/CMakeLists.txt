set(BINARY manager)

include_directories("${PROJECT_SOURCE_DIR}/include/${BINARY}/")

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

if (UNIX)
  target_link_libraries(${BINARY} PUBLIC processor helper CURL::libcurl)
elseif(WIN32)
  target_link_libraries(${BINARY} PUBLIC processor helper ${CURL_LIBRARY} wldap32 ws2_32 Crypt32.lib Wldap32 Normaliz)
endif()
