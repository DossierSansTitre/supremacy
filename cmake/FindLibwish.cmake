find_library(
    WISH_LIBRARY
    wish
)

find_path(
    WISH_INCLUDE_DIR
    wish.h
)

add_library(libwish STATIC IMPORTED)

set_target_properties(
    libwish
    PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES
    ${WISH_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES
    -lncurses
    IMPORTED_LOCATION
    ${WISH_LIBRARY}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(libwish WISH_INCLUDE_DIR WISH_LIBRARY)
