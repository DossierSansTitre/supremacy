find_package(GMock)

file(GLOB_RECURSE SOURCES "*.cpp")

add_executable(tests ${SOURCES})
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(tests GMock::GMock)
