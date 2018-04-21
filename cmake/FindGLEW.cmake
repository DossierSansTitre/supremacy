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
    libGLEW.a glew32 glew
    PATHS
	${GLEW_SEARCH_PATH}
    PATH_SUFFIXES
	lib
    )

if (WIN32)
	find_file(GLEW_DLL
	NAMES
	glew32.dll
	PATHS
	${GLEW_SEARCH_PATH}
	PATH_SUFFIXES
	bin
	)
endif()
if (GLEW_LIBRARY AND GLEW_INCLUDE_DIR)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)
