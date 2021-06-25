#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include "shader.hpp"

GLuint loadShader(const char *vertex_file_path, const char *fragment_file_path) {
    // create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // read the vertex shader code from the file
    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
    if (vertex_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << vertex_shader_stream.rdbuf();
        vertex_shader_code = sstr.str();
        vertex_shader_stream.close();
    } else {
        std::cerr << "Failed to open " << vertex_file_path << std::endl;
        return 0;
    }

    // read the fragment shader code from the file
    std::string fragment_shader_code;
    std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
    if (fragment_shader_stream.is_open()) {
        std::stringstream sstr;
        sstr << fragment_shader_stream.rdbuf();
        fragment_shader_code = sstr.str();
        fragment_shader_stream.close();
    } else {
        std::cerr << "Failed to open " << fragment_file_path << std::endl;
        return 0;
    }

    GLint result = GL_FALSE;
    int log_length;

    // compile vertex shader
    char const *vertex_source_ptr = vertex_shader_code.c_str();
    glShaderSource(vertex_shader_id, 1, &vertex_source_ptr, nullptr);
    glCompileShader(vertex_shader_id);

    // Check vertex shader
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        std::vector<char> error_message(log_length + 1);
        glGetShaderInfoLog(vertex_shader_id, log_length, nullptr, &error_message[0]);
        std::cout << &error_message[0] << std::endl;
    }

    // compile fragment shader
    char const *fragment_source_ptr = fragment_shader_code.c_str();
    glShaderSource(fragment_shader_id, 1, &fragment_source_ptr, nullptr);
    glCompileShader(fragment_shader_id);

    // check fragment shader
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        std::vector<char> error_message(log_length + 1);
        glGetShaderInfoLog(fragment_shader_id, log_length, nullptr, &error_message[0]);
        std::cout << &error_message[0] << std::endl;
    }

    // link the program
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // check the program
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    if (log_length > 0) {
        std::vector<char> error_message(log_length + 1);
        glGetProgramInfoLog(program_id, log_length, nullptr, &error_message[0]);
        std::cout << &error_message[0] << std::endl;
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}
