#include "gl_utils.h"

#include <cmath>
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

GLuint load_shader(const char* shader_file, GLenum shader_type) {

    const char* shader_type_str = (shader_type == GL_VERTEX_SHADER) ?
        "vert" : "frag";

    std::string shader_src;
    if (!get_file_content(shader_file, shader_src)) {
        std::cout << "couldn't open the " << shader_type_str << " shader file: " <<
            shader_file << std::endl;
        exit(-1);
    }

    const char* shader_src_cstr = shader_src.c_str();
    GLint compile_status;

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src_cstr, NULL);
    if (check_gl_err()) {
        print_shader_log(shader, shader_file);
        exit(-1);
    }
    glCompileShader(shader);
    if (check_gl_err()) {
        print_shader_log(shader, shader_file);
        exit(-1);
    }
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        print_shader_log(shader, shader_file);
        exit(-1);
    }

    return shader;
}

GLuint make_prog(const char* vert_shader_file, const char* frag_shader_file) {

    GLuint prog = glCreateProgram();

    GLuint vs = load_shader(vert_shader_file, GL_VERTEX_SHADER);
    GLuint fs = load_shader(frag_shader_file, GL_FRAGMENT_SHADER);

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

GLint get_uniform_loc(GLuint prog, const char* name) {
    GLint loc = glGetUniformLocation(prog, name);
    if (loc == -1) {
        std::cout << "couldn't get the uniform for " << name << std::endl;
        exit(-1);
    }
    return loc;
}

void get_identity_matrix(Matrix44f& result) {
    memset(result, 0, sizeof(result));
    for (int i = 0; i < 4; i++) {
        result[i][i] = 1;
    }
}

void mult_matrices(
    const Matrix44f& m1, const Matrix44f& m2,
    Matrix44f& result) {

    Matrix44f tmp_result = {0};

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            for (int i = 0; i < 4; i++) {
                tmp_result[x][y] += m1[x][i] * m2[i][y];
            }
        }
    }

    memcpy(result, tmp_result, sizeof(result));
}

void add_y_rotation_to_matrix(Matrix44f& m, float rot) {
    Matrix44f rot_matrix;
    get_identity_matrix(rot_matrix);

    float s = sin(rot);
    float c = cos(rot);

    rot_matrix[0][0] = c;
    rot_matrix[2][0] = s;
    rot_matrix[0][2] = -s;
    rot_matrix[2][2] = c;

    mult_matrices(m, rot_matrix, m);
}

void get_perspective_info(
    float fov, float aspect_ratio, float near, float far,
    float& b, float& t, float& l, float& r)
{
    float scale = tan(fov * 0.5 * PI / 180) * near;
    r = aspect_ratio * scale;
    l = -r;
    t = scale;
    b = -t;
}

void calc_proj_matrix(
    float b, float t, float l, float r, float n, float f,
    Matrix44f& mat)
{
    // set OpenGL perspective projection matrix
    mat[0][0] = 2 * n / (r - l);
    mat[0][1] = 0;
    mat[0][2] = 0;
    mat[0][3] = 0;

    mat[1][0] = 0;
    mat[1][1] = 2 * n / (t - b);
    mat[1][2] = 0;
    mat[1][3] = 0;

    mat[2][0] = (r + l) / (r - l);
    mat[2][1] = (t + b) / (t - b);
    mat[2][2] = -(f + n) / (f - n);
    mat[2][3] = -1;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = -2 * f * n / (f - n);
    mat[3][3] = 0;
}

void calc_lookat_matrix(
    const Vec3f& pos, const Vec3f& at, const Vec3f& up,
    Matrix44f& mat) {

    Vec3f z_axis = (pos - at).norm();

    Vec3f x_axis = up.cross(z_axis).norm();
    Vec3f y_axis = z_axis.cross(x_axis);

    mat[0][0] = x_axis.x;
    mat[1][0] = x_axis.y;
    mat[2][0] = x_axis.z;
    mat[3][0] = x_axis.dot(pos) * -1.0f;

    mat[0][1] = y_axis.x;
    mat[1][1] = y_axis.y;
    mat[2][1] = y_axis.z;
    mat[3][1] = y_axis.dot(pos) * -1.0f;

    mat[0][2] = z_axis.x;
    mat[1][2] = z_axis.y;
    mat[2][2] = z_axis.z;
    mat[3][2] = z_axis.dot(pos) * -1.0f;

    mat[0][3] = 0;
    mat[1][3] = 0;
    mat[2][3] = 0;
    mat[3][3] = 1;
}
