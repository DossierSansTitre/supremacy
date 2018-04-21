cmake_minimum_required(VERSION 3.9)

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++1z -fno-exceptions -fno-rtti -fstrict-aliasing -fno-stack-protector")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

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

add_subdirectory(src)
#add_subdirectory(test)
