cmake_minimum_required (VERSION 3.5)

# Debug specifics
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Release specifics
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# Configuration specific defines
include(configuration/${AFM_CONFIG}.cmake)

set (CMAKE_CXX_STANDARD 14)

set (JSON_INCLUDE /usr/local/include/nlohmann)
set (COMMON_INCLUDE ${CMAKE_SOURCE_DIR}/common)

# Output for install
set (LIB_DIR ${CMAKE_BINARY_DIR}/staging/lib)
set (BIN_DIR ${CMAKE_BINARY_DIR}/staging/bin)

add_subdirectory(communications)

