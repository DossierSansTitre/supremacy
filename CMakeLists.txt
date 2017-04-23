cmake_minimum_required(VERSION 3.0)
project(supremacy)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++14 -fstrict-aliasing")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

configure_file("${CMAKE_SOURCE_DIR}/src/config.h.in" "${CMAKE_BINARY_DIR}/include/config.h")

find_package(OpenGL)
find_package(SDL2)

file(GLOB_RECURSE SOURCES "src/*.cpp")
include_directories(src)
include_directories(${CMAKE_BINARY_DIR}/include)

add_executable(supremacy ${SOURCES})
target_link_libraries(supremacy sdl2 ${OPENGL_LIBRARIES})

install(TARGETS supremacy DESTINATION bin)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/data/" DESTINATION "share/supremacy")
