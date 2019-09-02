cmake_minimum_required (VERSION 3.5)

set(COMMON_LIB_FILES
    src/processLock.cpp
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

add_library (AFMCommon ${COMMON_LIB_FILES})

