find_package(GTest REQUIRED)

set(GMOCK_PATHS
    /usr/local
    /opt/local
    /usr
    "/Project Files/gmock"
    ${GMOCK_DIR}
    ${GTEST_DIR}
)

find_path(
    GMOCK_INCLUDE_DIR
    gmock/gmock.h
    PATHS
    ${GMOCK_PATHS}
    PATH_SUFFIXES
    include
)

find_library(
    GMOCK_LIBRARY
    NAMES
    libgmock.a
    PATHS
    ${GMOCK_PATHS}
    PATH_SUFFIXES
    lib
)

find_library(
    GMOCK_MAIN
    NAMES
    libgmock_main.a
    PATHS
    ${GMOCK_PATHS}
    PATH_SUFFIXES
    lib
)

add_library(GMock::GMock STATIC IMPORTED)
add_library(GMock::Main STATIC IMPORTED)

set_target_properties(
    GMock::GMock
    PROPERTIES
    IMPORTED_LOCATION "${GMOCK_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GMOCK_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES GTest::GTest
)

set_target_properties(
    GMock::Main
    PROPERTIES
    IMPORTED_LOCATION "${GMOCK_MAIN}"
    INTERFACE_LINK_LIBRARIES GMock::GMock
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMock DEFAULT_MSG GMOCK_LIBRARY GMOCK_INCLUDE_DIR GMOCK_MAIN)
