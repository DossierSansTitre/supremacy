set(GTEST_PATHS
    /usr/local
    /opt/local
    /usr
    "/Project Files/gtest"
    ${GTEST_DIR}
)

find_path(
    GTEST_INCLUDE_DIR
    gtest/gtest.h
    PATHS
    ${GTEST_PATHS}
    PATH_SUFFIXES
    include
)

find_library(
    GTEST_LIBRARY
    NAMES
    libgtest.a
    PATHS
    ${GTEST_PATHS}
    PATH_SUFFIXES
    lib
)

find_library(
    GTEST_MAIN
    NAMES
    libgtest_main.a
    PATHS
    ${GTEST_PATHS}
    PATH_SUFFIXES
    lib
)

add_library(GTest::GTest STATIC IMPORTED)
add_library(GTest::Main STATIC IMPORTED)

set_target_properties(
    GTest::GTest
    PROPERTIES
    IMPORTED_LOCATION "${GTEST_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIR}"
)

set_target_properties(
    GTest::Main
    PROPERTIES
    IMPORTED_LOCATION "${GTEST_MAIN}"
    INTERFACE_LINK_LIBRARIES GTest::GTest
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTest DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIR GTEST_MAIN)
