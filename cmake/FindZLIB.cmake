set(ZLIB_FOUND "NO")

find_path(
    ZLIB_INCLUDE_DIR
    zlib.h
    PATHS
    /usr/local/include
    /opt/local/include
    /usr/include
    ${ZLIB_DIR}/include
    )

find_library(ZLIB_LIBRARY
    NAMES
    zlib
    libz
    PATHS
    /usr/lib64
    /usr/lib
    /usr/local/lib
    ${ZLIB_DIR}/lib
    )

if (WIN32)
    find_file(ZLIB_DLL
        NAMES
        zlib.dll
        PATHS
        ${ZLIB_DIR}/bin
        )
endif()

if (ZLIB_LIBRARY AND ZLIB_INCLUDE_DIR)
    set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
    set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
    set(ZLIB_FOUND "YES")
endif()


if (ZLIB_FOUND)
    if (NOT ZLIB_FIND_QUIETLY)
        message(STATUS "Found ZLIB: ${ZLIB_LIBRARY}")
    endif()
else()
    if (ZLIB_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find ZLIB library")
    endif()
endif()

mark_as_advanced(ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
