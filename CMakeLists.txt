cmake_minimum_required(VERSION 3.0)
project(supremacy)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -fno-exceptions -fno-rtti -fstrict-aliasing -fno-stack-protector")
endif()

if (APPLE)
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

if (WIN32)
    find_package(GLEW)
endif()

find_package(OpenGL)
find_package(SDL2)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/include)

if (WIN32)
    include_directories(${GLEW_INCLUDE_DIR} SYSTEM)
endif()

add_executable(supremacy ${SOURCES} ${HEADERS})
target_link_libraries(supremacy sdl2 ${OPENGL_LIBRARIES})
if (WIN32)
    target_link_libraries(supremacy ${GLEW_LIBRARY})
endif()

file(GLOB_RECURSE SUP_DATA "${CMAKE_SOURCE_DIR}/data/*")
file(GLOB_RECURSE SUP_TOOLS "${CMAKE_SOURCE_DIR}/util/*")

add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin"
    DEPENDS ${SUP_DATA} ${SUP_TOOLS}
    COMMAND "${CMAKE_SOURCE_DIR}/util/mkdata" "${CMAKE_SOURCE_DIR}/data" "${CMAKE_BINARY_DIR}/${DATA_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/data/tileset.bmp" "${CMAKE_BINARY_DIR}/${DATA_DIR}"
    COMMENT "Generating supremacy.bin"
    VERBATIM
    )

add_custom_target(
    data
    ALL
    DEPENDS "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin"
    VERBATIM
)

file(GLOB_RECURSE INSTALL_DATA "${CMAKE_SOURCE_DIR}/data/*.bmp")

install(TARGETS supremacy DESTINATION ${RUNTIME_DIR})
install(FILES ${INSTALL_DATA} DESTINATION ${DATA_DIR})
install(FILES "${CMAKE_BINARY_DIR}/${DATA_DIR}/supremacy.bin" DESTINATION ${DATA_DIR})
