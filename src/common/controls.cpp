#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>

#include "controls.hpp"

glm::mat4 proj_matrix;
glm::mat4 view_matrix;

glm::vec3 position = glm::vec3(0, 0, 5);
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFoV = 45.0f;

float speed = 3.0f;
float mouseSpeed = 0.05f;

float FoV = initialFoV - 5;

void handle_scroll(GLFWwindow* window, double x, double y) {
    FoV -= (float) y;
}

void computeMatricesFromInputs(GLFWwindow* window) {
    glfwSetScrollCallback(window, handle_scroll);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    static double lastTime = glfwGetTime();

    double currentTime = glfwGetTime();
    auto deltaTime = float(lastTime - currentTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, width / 2., height / 2.);

    horizontalAngle += mouseSpeed * deltaTime * float(xpos - width / 2.);
    verticalAngle += mouseSpeed * deltaTime * float(ypos - height / 2.);

    glm::vec3 direction(
            glm::cos(verticalAngle) * glm::sin(horizontalAngle),
            glm::sin(verticalAngle),
            glm::cos(verticalAngle) * glm::cos(horizontalAngle)
    );

    glm::vec3 right = glm::vec3(
            glm::sin(horizontalAngle - glm::pi<float>() / 2.0f),
            0,
            glm::cos(horizontalAngle - glm::pi<float>() / 2.0f)
    );

    glm::vec3 up = glm::cross(right, direction);

    if (glfwGetKey(window, GLFW_KEY_W)) {
        position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        position += right * deltaTime * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        position -= right * deltaTime * speed;
    }

    proj_matrix = glm::perspective(glm::radians(FoV), (float) width / (float) height, 0.1f, 100.0f);
    view_matrix = glm::lookAt(position, position + direction, up);

    lastTime = currentTime;
}

glm::mat4 getProjectionMatrix() {
    return proj_matrix;
}

glm::mat4 getViewMatrix() {
    return view_matrix;
}
