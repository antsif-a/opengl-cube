#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "common/shader.hpp"
#include "common/controls.hpp"

#pragma clang diagnostic ignored "-Wmissing-braces"

struct Vertex {
    float x, y, z;
};

struct Color {
    float r, g, b;
};

static GLFWwindow *window;
static GLuint vertex_arr_id, vertex_buffer, color_buffer, program_id, matrix_id;

static const int total_vertices = 6 * 2 * 3; // 6 - squares for cube; 2 - triangles for square; 3 - vertices in triangle

// vertices for triangles
static struct Vertex vertex_buffer_data[total_vertices * 3] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

static struct {
    float r, g, b;
} color_buffer_data[total_vertices * 3] = {};

void gen_color_buffer_data() {
    for (int v = 0; v < total_vertices; v++) {
        color_buffer_data[v].r = vertex_buffer_data[v].x;
        color_buffer_data[v].g = vertex_buffer_data[v].y;
        color_buffer_data[v].b = vertex_buffer_data[v].z;
    }
}

void init() {
    gen_color_buffer_data();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    program_id = loadShader("shader.vert", "shader.frag");

    glGenVertexArrays(1, &vertex_arr_id);
    glBindVertexArray(vertex_arr_id);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

    matrix_id = glGetUniformLocation(program_id, "MVP");
}

int main() {
    glfwSetErrorCallback([] (int error_core, const char *description) -> void {
        fprintf(stderr, description);
    });

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        system("pause");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(mode->width, mode->height, "The Cube", monitor, nullptr);
    if (window == nullptr) {
        glfwTerminate();

        std::cerr << "Failed to open GLFW window" << std::endl;
        system("pause");
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();

        std::cerr << "Failed to initialize GLEW" << std::endl;
        system("pause");
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);

    init();

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_id);

        computeMatricesFromInputs(window);
        glm::mat4 proj_matrix = getProjectionMatrix();
        glm::mat4 view_matrix = getViewMatrix();
        glm::mat4 model_matrix = glm::mat4(1.0);
        glm::mat4 mvp = proj_matrix * view_matrix * model_matrix;

        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteProgram(program_id);
    glDeleteVertexArrays(1, &vertex_arr_id);

    glfwTerminate();

    return 0;
}
