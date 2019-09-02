cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Configuration specific defines
include(configuration/${AFM_CONFIG}.cmake)

set (CMAKE_CXX_STANDARD 14)

set (JSON_INCLUDE /usr/local/include/nlohmann)

# Output for install
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

# Common defines
set(COMMON_INCLUDE ${CMAKE_SOURCE_DIR}/common)
set(COMMON_LIB ${CMAKE_BINARY_DIR}/common)

# Communications defines
set(SOCKET_INCLUDE ${CMAKE_SOURCE_DIR}/communications/socket)
set(SOCKET_LIB ${CMAKE_BINARY_DIR}/communications/socket)
set(MQTT_INCLUDE ${CMAKE_SOURCE_DIR}/communications/mqtt)
set(MQTT_LIB ${CMAKE_BINARY_DIR}/communications/mqtt)

# Now for each subdirectory - add after defines above
add_subdirectory(common)
add_subdirectory(communications)
