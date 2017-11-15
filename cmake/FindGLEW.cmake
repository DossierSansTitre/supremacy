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

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR)
