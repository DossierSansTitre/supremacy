set(ZLIB_FOUND "NO")

find_path(
    ZLIB_INCLUDE_DIR
    zlib.h
    PATHS
    /usr/local/include
    /opt/local/include
    /usr/include
    "/Program Files/zlib/include"
    ${ZLIB_DIR}/include
    )

find_library(ZLIB_LIBRARY
    NAMES
    zlib
    z
    PATHS
    /usr/local/lib64
    /usr/local/lib
    /opt/local/lib
    /usr/lib64
    /usr/lib
    "/Program Files/zlib/lib"
    ${ZLIB_DIR}/lib
    )

if (WIN32)
    find_file(ZLIB_DLL
        NAMES
        zlib.dll
        PATHS
        "/Program Files/zlib/bin"
        ${ZLIB_DIR}/bin
        )
endif()

if (ZLIB_LIBRARY AND ZLIB_INCLUDE_DIR)
    set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
    set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
    set(ZLIB_FOUND "YES")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZLIB REQUIRED_VARS ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
