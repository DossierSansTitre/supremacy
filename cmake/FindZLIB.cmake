set(ZLIB_SEARCH_PATHS
    "/usr/local"
    "/opt/local"
    "/usr"
    "/"
    "/Program Files/zlib")

find_path(
    ZLIB_INCLUDE_DIR
    zlib.h
    PATHS
    ${ZLIB_DIR}
    ${ZLIB_SEARCH_PATHS}
    PATH_SUFFIXES
    include
    )

find_library(ZLIB_LIBRARY
    NAMES
    z zlib
    PATHS
    ${ZLIB_DIR}
    ${ZLIB_SEARCH_PATHS}
    PATH_SUFFIXES
    lib
    NO_DEFAULT_PATH
    )

if (WIN32)
    find_file(ZLIB_DLL
        NAMES
        zlib.dll
        PATHS
        ${ZLIB_DIR}
        ${ZLIB_SEARCH_PATHS}
        PATH_SUFFIXES
        lib
        )
endif()

if (ZLIB_LIBRARY AND ZLIB_INCLUDE_DIR)
    set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
    set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
