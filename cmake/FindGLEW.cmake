set(GLEW_SEARCH_PATH
	/usr/local
	/opt/local
	/usr
	"/Program Files/GLEW"
	${GLEW_DIR}
)

find_path(
    GLEW_INCLUDE_DIR
    GL/glew.h
    PATHS
    ${GLEW_SEARCH_PATH}
	PATH_SUFFIXES
	include
    )

find_library(GLEW_LIBRARY
    NAMES
    libGLEW.a glew32s glew
    PATHS
	${GLEW_SEARCH_PATH}
    PATH_SUFFIXES
	lib
    )

if (GLEW_LIBRARY AND GLEW_INCLUDE_DIR)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)
