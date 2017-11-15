find_package(ZLIB REQUIRED)

SET(PNG_FOUND "NO")

if (ZLIB_FOUND)
    find_path(
        PNG_PNG_INCLUDE_DIR
        png.h
        PATHS
        /usr/local/include
        /opt/local/include
        /usr/include
        "/Program Files/libpng/include"
        ${PNG_DIR}/include
        NO_DEFAULT_PATH
        )

    SET(PNG_NAMES ${PNG_NAMES} png libpng libpng16)
    FIND_LIBRARY(PNG_LIBRARY
        NAMES ${PNG_NAMES}
        PATHS
        /usr/lib64
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        "/Program Files/libpng/lib"
        ${PNG_DIR}/lib
        )

    if (WIN32)
        find_file(PNG_DLL
            NAMES libpng.dll libpng16.dll
            PATHS
            "/Program Files/libpng/bin"
            ${PNG_DIR}/bin
            )
    endif()

    IF (PNG_LIBRARY AND PNG_INCLUDE_DIR)
        # png.h includes zlib.h. Sigh.
        SET(PNG_INCLUDE_DIRS ${PNG_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR} )
        SET(PNG_LIBRARIES ${PNG_LIBRARY} ${ZLIB_LIBRARY})
        SET(PNG_FOUND "YES")
        SET(HAVE_PNG_H)
        IF (CYGWIN)
            IF(BUILD_SHARED_LIBS)
                # No need to define PNG_USE_DLL here, because it's default for Cygwin.
            ELSE(BUILD_SHARED_LIBS)
                SET (PNG_DEFINITIONS -DPNG_STATIC)
            ENDIF(BUILD_SHARED_LIBS)
        ENDIF (CYGWIN)
    ENDIF ()
ENDIF()

MARK_AS_ADVANCED(PNG_PNG_INCLUDE_DIR PNG_LIBRARY )
