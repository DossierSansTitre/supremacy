set(GLEW_FOUND "NO")

find_path(
    GLEW_INCLUDE_DIR
    GL/glew.h
    PATHS
    /usr/local/include
    /opt/local/include
    /usr/include
    "/Program Files/GLEW/include"
    ${GLEW_DIR}/include
    )

find_library(GLEW_LIBRARY
    NAMES
    GLEW libGLEW.a GLEW
    PATHS
    /usr/local/lib
    /opt/local/lib
    /usr/lib
    "/Program Files/GLEW/lib"
    ${GLEW_DIR}/lib
    )

if (GLEW_LIBRARY AND GLEW_INCLUDE_DIR)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
    set(GLEW_FOUND "YES")
endif()


if (GLEW_FOUND)
    if (NOT GLEW_FIND_QUIETLY)
        message(STATUS "Found GLEW: ${GLEW_LIBRARY}")
    endif()
else()
    if (GLEW_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find GLEW library")
    endif()
endif()

mark_as_advanced(GLEW_LIBRARY GLEW_INCLUDE_DIR)
