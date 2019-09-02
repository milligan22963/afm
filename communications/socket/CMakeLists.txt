cmake_minimum_required (VERSION 3.5)

set(SOCKET_LIB_FILES
    src/socket.cpp
    src/clientSocket.cpp
    src/serverSocket.cpp
    src/socketFactory.cpp
)

set(SOCKET_SRC_FILES
    test/main.cpp
)

include_directories (
    .
    internal
    ${COMMON_INCLUDE}
    ${JSON_INCLUDE}
)

link_directories (
    ${LIB_DIR}
)

add_library (AFMSocket ${SOCKET_LIB_FILES})
add_executable (AFMSocketClientTest ${SOCKET_SRC_FILES})

target_link_libraries(
    AFMSocketClientTest
    AFMSocket
    pthread
)

install (TARGETS AFMSocketClientTest DESTINATION ${BIN_DIR})

