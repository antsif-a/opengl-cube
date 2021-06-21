cmake_minimum_required(VERSION 3.10)

project(App VERSION 1.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(App src/main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)

target_include_directories(App PRIVATE ${OPENGL_INCLUDE_DIRS})
target_include_directories(App PRIVATE ${GLFW_INCLUDE_DIRS})

target_link_libraries(App ${GLFW_LIBRARIES})
target_link_libraries(App ${OPENGL_LIBRARIES})
