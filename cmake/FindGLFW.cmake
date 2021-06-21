set(GLFW_DIR ${CMAKE_SOURCE_DIR}/libs/glfw)

if (NOT EXISTS ${GLFW_DIR})
    message(FATAL_ERROR "Cannot find GLFW installation, try running 'git submodule update --init'.")
endif()

add_subdirectory(${GLFW_DIR})

set(GLFW_DEPS ${GLFW_DIR}/deps)
AUX_SOURCE_DIRECTORY(${GLFW_DEPS} GLFW_DEPS_SOURCES)

set(GLFW_INCLUDE_DIRS ${GLFW_DIR}/include ${GLFW_DEPS})
set(GLFW_LIBRARIES glfw)

target_sources(App PRIVATE ${GLFW_DEPS_SOURCES})
