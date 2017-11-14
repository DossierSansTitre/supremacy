cmake_minimum_required(VERSION 3.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.7")

project(supremacy C CXX)
set(VERSION "0.1.0")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if (APPLE)
    option(APP_BUNDLE "Build a macOS bundle application")
endif()
option(NO_THREADING "Disable the thread pool")

if (APP_BUNDLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()

if (NO_THREADING)
    add_definitions(-DNO_THREADING=1)
endif()

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -fno-exceptions -fno-rtti -fstrict-aliasing -fno-stack-protector -ffast-math")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (WIN32)
    set(RUNTIME_DIR ".")
    set(DATA_DIR "data")
else()
    set(RUNTIME_DIR "bin")
    set(DATA_DIR "share/supremacy")
endif()

configure_file("${CMAKE_SOURCE_DIR}/src/config.h.in" "${CMAKE_BINARY_DIR}/include/config.h")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(PNG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE SOURCES_MAC "src/*.mm" "src/*_Mac.cpp")
file(GLOB_RECURSE SOURCES_LINUX "src/*_Linux.cpp")
file(GLOB_RECURSE SOURCES_WIN32
    "src/*_Win32.cpp"
    "src/window/win32.cpp")
file(GLOB_RECURSE SOURCES_UNIX "src/*_UNIX.cpp")

list(APPEND SOURCES_MAC ${SOURCES_UNIX})
list(APPEND SOURCES_LINUX ${SOURCES_UNIX})

list(REMOVE_ITEM SOURCES ${SOURCES_MAC} ${SOURCES_LINUX} ${SOURCES_WIN32})

file(GLOB_RECURSE HEADERS "include/*.h")

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
    add_definitions(-D_UNICODE=1)
    add_definitions(-DUNICODE=1)
    list(APPEND SOURCES ${SOURCES_WIN32})
elseif (APPLE)
    list(APPEND SOURCES ${SOURCES_MAC})
else()
    list(APPEND SOURCES ${SOURCES_LINUX})
endif()

include_directories(include)
include_directories(${PNG_INCLUDE_DIR})
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${GLEW_INCLUDE_DIR} SYSTEM)

if (WIN32)
    source_group(TREE ${CMAKE_SOURCE_DIR}/src PREFIX src FILES ${SOURCES})
    source_group(TREE ${CMAKE_SOURCE_DIR}/include PREFIX include FILES ${HEADERS})
endif()

if(APP_BUNDLE)
    set(MACOSX_BUNDLE_ICON_FILE Supremacy.icns)
    set(SUPREMACY_ICON "${CMAKE_SOURCE_DIR}/data/Supremacy.icns")
    set_source_files_properties(${SUPREMACY_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(supremacy MACOSX_BUNDLE ${SOURCES} ${HEADERS} "${SUPREMACY_ICON}")
else()
    add_executable(supremacy ${SOURCES} ${HEADERS})
endif()

target_link_libraries(supremacy ${PNG_LIBRARIES} sdl2 ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

file(GLOB_RECURSE SUP_DATA "${CMAKE_SOURCE_DIR}/data/*")
file(GLOB_RECURSE SUP_TOOLS "${CMAKE_SOURCE_DIR}/util/*")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin"
    DEPENDS ${SUP_DATA} ${SUP_TOOLS}
    COMMAND "ruby" "${CMAKE_SOURCE_DIR}/util/mkdata" "${CMAKE_SOURCE_DIR}/data" "${CMAKE_BINARY_DIR}/${DATA_DIR}"
    COMMENT "Generating supremacy.bin"
    VERBATIM
    )

add_custom_target(
    data
    ALL
    DEPENDS "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin"
    VERBATIM
    )

if (APP_BUNDLE)
    set_target_properties(supremacy PROPERTIES OUTPUT_NAME "Supremacy" MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/data/Info.plist")

    add_custom_command(
        OUTPUT  "${CMAKE_BINARY_DIR}/Supremacy.app/Contents/Resources/supremacy.bin"
        DEPENDS "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin"
        supremacy
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/Supremacy.app/Contents/Resources/"
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin" "${CMAKE_BINARY_DIR}/Supremacy.app/Contents/Resources/"
        COMMENT "Copying supremacy.bin into bundle"
        VERBATIM
        )

    add_custom_target(
        data-bundle
        ALL
        DEPENDS "${CMAKE_BINARY_DIR}/Supremacy.app/Contents/Resources/supremacy.bin"
        VERBATIM
        )
endif()

if (APP_BUNDLE)
    install(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${CMAKE_BINARY_DIR}/Supremacy.app\" \"\" \"\")
    ")
    install(DIRECTORY "${CMAKE_BINARY_DIR}/Supremacy.app" DESTINATION "/Applications" USE_SOURCE_PERMISSIONS)
else()
    install(TARGETS supremacy DESTINATION ${RUNTIME_DIR})
    install(FILES "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin" DESTINATION ${DATA_DIR})
endif()

if (WIN32)
    add_custom_command(TARGET supremacy POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        ${SDL2_DLL}
        ${PNG_DLL}
        ${ZLIB_DLL}
        $<TARGET_FILE_DIR:supremacy>)
    add_custom_command(TARGET data POST_BUILD
        COMMAND
        ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:supremacy>/data/
        COMMAND
        ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin
        $<TARGET_FILE_DIR:supremacy>/data/supremacy.bin)
endif()

if (NOT WIN32)
    set(CPACK_PACKAGE_VERSION ${VERSION})
    if (APPLE)
        set(CPACK_GENERATOR "TGZ")
    else()
        set(CPACK_GENERATOR "TGZ;RPM;DEB")
    endif()
    set(CPACK_PACKAGE_NAME "supremacy")
    set(CPACK_PACKAGE_RELEASE 1)
    set(CPACK_PACKAGE_CONTACT "Maxime Bacoux")
    set(CPACK_PACKAGE_VENDOR "Dossier Sans Titre")
    set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
    include(CPack)
endif()
