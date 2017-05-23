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
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if (WIN32)
	set(RUNTIME_DEST ".")
	set(DATA_DEST "data")
else()
	set(RUNTIME_DEST "bin")
	set(DATA_DEST "share/supremacy")
endif()

configure_file("${CMAKE_SOURCE_DIR}/src/config.h.in" "${CMAKE_BINARY_DIR}/include/config.h")

if (WIN32)
	find_package(GLEW)
endif()

find_package(OpenGL)
find_package(SDL2)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

include_directories(src)
include_directories(${CMAKE_BINARY_DIR}/include)

if (WIN32)
	include_directories(${GLEW_INCLUDE_DIR} SYSTEM)
endif()

add_executable(supremacy ${SOURCES} ${HEADERS})
target_link_libraries(supremacy sdl2 ${OPENGL_LIBRARIES})
if (WIN32)
	target_link_libraries(supremacy ${GLEW_LIBRARY})
endif()

install(TARGETS supremacy DESTINATION ${RUNTIME_DEST})
install(DIRECTORY "${CMAKE_SOURCE_DIR}/data/" DESTINATION ${DATA_DEST})
