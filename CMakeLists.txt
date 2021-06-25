cmake_minimum_required(VERSION 3.1)

project(OpenGL-Cube VERSION 1.0)

add_subdirectory(libs)

add_executable(main
  src/main.cpp
  src/common/shader.cpp src/common/shader.hpp
  src/common/controls.cpp src/common/controls.hpp)

target_link_libraries(main glfw glew glm)

set_target_properties(main PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO)

set(RESOURCE_FILES
  resources/shader.vert
  resources/shader.frag)

foreach(RESOURCE IN LISTS RESOURCE_FILES)
    configure_file(${RESOURCE} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
