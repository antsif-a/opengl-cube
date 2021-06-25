set(GLEW_DIR ${CMAKE_SOURCE_DIR}/libs/glew)
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/libs/glfw)
set(GLM_DIR ${CMAKE_SOURCE_DIR}/libs/glm)

find_package(OpenGL REQUIRED)

### GLFW ###

if (NOT EXISTS ${GLFW_DIR})
  message(FATAL_ERROR "Cannot find GLFW installation, try running 'git submodule update --init'.")
endif()

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)

add_subdirectory(${GLFW_DIR})

set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)
set(GLFW_LIBRARIES glfw)

### GLEW ###

add_subdirectory(${GLEW_DIR})
add_library(glew STATIC ${GLEW_DIR}/src/glew.c)

set(GLEW_INCLUDE_DIR ${GLEW_DIR}/include)
set(GLEW_LIBRARIES glew)

target_compile_definitions(glew PUBLIC GLEW_STATIC)
target_include_directories(glew PUBLIC ${GLEW_INCLUDE_DIR})
target_link_libraries(glew PRIVATE OpenGL::GL)

### GLM ###

add_subdirectory(${GLM_DIR})
