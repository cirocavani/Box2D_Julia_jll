cmake_minimum_required(VERSION 3.8)

# https://cmake.org/cmake/help/latest/command/project.html
project(box2d_julia VERSION 2.4.1)

cmake_minimum_required(VERSION 3.8)
set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

find_package(JlCxx)
get_target_property(JlCxx_location JlCxx::cxxwrap_julia LOCATION)
get_filename_component(JlCxx_location ${JlCxx_location} DIRECTORY)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib;${JlCxx_location}")

message(STATUS "Found JlCxx at ${JlCxx_location}")

add_library(box2d_julia SHARED box2d_julia.cpp)

target_link_libraries(box2d_julia JlCxx::cxxwrap_julia box2d)

install(TARGETS
  box2d_julia
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION lib
)