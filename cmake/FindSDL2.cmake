find_path(
    SDL2_INCLUDE_DIR
    SDL2/SDL.h
    PATHS
    "/Program Files/SDL2"
    ${SDL2_DIR}
    PATH_SUFFIXES
    include
)

find_library(
    SDL2_LIBRARY
    NAMES
    SDL2
    PATHS
    "/Program Files/SDL2"
    ${SDL2_DIR}
    PATH_SUFFIXES
    lib64
    lib
)

find_library(
    SDL2MAIN_LIBRARY
    NAMES
    SDL2main
    PATHS
    "/Program Files/SDL2"
    ${SDL2_DIR}
    PATH_SUFFIXES
    lib64
    lib
)

if(WIN32)
    find_path(
        SDL2_DLL_TMP
        SDL2.dll
        PATHS
        "/Program Files/SDL2"
        ${SDL2_DIR}
        PATH_SUFFIXES
        bin
    )
set(SDL2_DLL ${SDL2_DLL_TMP}/SDL2.dll)
endif()

if(NOT APPLE)
    find_package(Threads)
endif()

if(SDL2_LIBRARY)
    add_library(sdl2main STATIC IMPORTED)
    add_library(sdl2 SHARED IMPORTED)

    set_target_properties(
        sdl2main
        PROPERTIES
        IMPORTED_LOCATION ${SDL2MAIN_LIBRARY}
    )

    if(WIN32)
        set_target_properties(
            sdl2
            PROPERTIES
            IMPORTED_LOCATION   ${SDL2_DLL}
            IMPORTED_IMPLIB     ${SDL2_LIBRARY}
        )
    else()
        set_target_properties(
            sdl2
            PROPERTIES
            IMPORTED_LOCATION   ${SDL2_LIBRARY}
        )
    endif()
    set(SDL2_DEPENDENCIES sdl2main)
    if(APPLE)
        list(APPEND SDL2_DEPENDENCIES "-framework Cocoa")
    else()
        list(APPEND SDL2_DEPENDENCIES ${CMAKE_THREAD_LIBS_INIT})
    endif()
    set_target_properties(
        sdl2
        PROPERTIES
        IMPORTED_LINK_INTERFACE_LIBRARIES "${SDL2_DEPENDENCIES}"
        INTERFACE_INCLUDE_DIRECTORIES ${SDL2_INCLUDE_DIR}
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)
