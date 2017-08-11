#include "gl_utils.h"

#include <cstdio>

#include <fstream>
#include <iostream>

#include <OpenGL/glu.h>

bool check_gl_err() {
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
        return false;
    }

    const char* err_msg = nullptr;

    switch (err) {
        case GL_INVALID_ENUM:
            err_msg = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            err_msg = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            err_msg = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            err_msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            err_msg = "GL_OUT_OF_MEMORY";
            break;
        case GL_NO_ERROR:
        default:
            break;
    }

    if (err_msg) {
        std::cout << err_msg << std::endl;
    }

    return true;
}

void check_gl_err_or_die(const char* msg) {
    if (check_gl_err()) {
        std::cout << msg << std::endl;
        exit(-1);
    }
}

static void print_shader_log(GLuint shader, const char* filename) {
    GLint max_log_length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_log_length);
    char* log = new char[max_log_length];

    GLint log_len;
    glGetShaderInfoLog(shader, max_log_length, &log_len, log);

    std::cout << "Shader " << filename << " log file:" << std::endl;
    if (*log) {
        std::cout << log << std::endl;
    }

    delete [] log;
}

static void print_program_log(GLuint prog) {
    GLint max_log_length;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &max_log_length);
    char* log = new char[max_log_length];

    GLint log_len;
    glGetProgramInfoLog(prog, max_log_length, &log_len, log);

    std::cout << "Program log file:" << std::endl;
    if (*log) {
        std::cout << log << std::endl;
    }

    delete [] log;
}

bool get_file_content(const std::string& path, std::string& content) {
    std::ifstream file(path);
    if (!file)
    {
        return false;
    }

    content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return true;
}

GLuint make_prog(const char* vert_shader_file, const char* frag_shader_file) {
    std::string vert_shader_src;
    std::string frag_shader_src;

    if (!get_file_content(vert_shader_file, vert_shader_src)) {
        std::cout << "couldn't open the vert shader file: " << vert_shader_file << std::endl;
        return 0;
    }

    if (!get_file_content(frag_shader_file, frag_shader_src)) {
        std::cout << "couldn't open the frag shader file: " << frag_shader_file << std::endl;
        return 0;
    }

    GLuint prog = glCreateProgram();

    const char* vert_shader_src_cstr = vert_shader_src.c_str();
    const char* frag_shader_src_cstr = frag_shader_src.c_str();

    GLint compile_status;

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vert_shader_src_cstr, NULL);
    if (check_gl_err()) {
        print_shader_log(vs, vert_shader_file);
        return 0;
    }
    glCompileShader(vs);
    if (check_gl_err()) {
        print_shader_log(vs, vert_shader_file);
        return 0;
    }
    glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        print_shader_log(vs, vert_shader_file);
        return 0;
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_shader_src_cstr, NULL);
    if (check_gl_err()) {
        print_shader_log(fs, frag_shader_file);
        return 0;
    }
    glCompileShader(fs);
    if (check_gl_err()) {
        print_shader_log(fs, frag_shader_file);
        return 0;
    }
    glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        print_shader_log(fs, frag_shader_file);
        return 0;
    }

    glAttachShader(prog, vs);
    if (check_gl_err()) {
        print_shader_log(vs, vert_shader_file);
        return 0;
    }
    glAttachShader(prog, fs);
    if (check_gl_err()) {
        print_shader_log(fs, frag_shader_file);
        return 0;
    }

    glLinkProgram(prog);
    if (check_gl_err()) {
        print_program_log(prog);
        return 0;
    }

    return prog;
}
