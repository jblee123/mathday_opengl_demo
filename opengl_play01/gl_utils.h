#pragma once

#include <cassert>
#include <cmath>

#include <GL/glew.h>

#include "vec_stuff.h"

#define PI 3.141592653589793
#define PI_F ((float)3.141592653589793)

typedef float Matrix44f[4][4];

void get_identity_matrix(Matrix44f& result);

void mult_matrices(
    const Matrix44f& m1, const Matrix44f& m2,
    Matrix44f& result);

void add_y_rotation_to_matrix(Matrix44f& m, float rot);

void get_perspective_info(
    float fov, float aspect_ratio, float near, float far,
    float& b, float& t, float& l, float& r);
void calc_proj_matrix(
    float b, float t, float l, float r, float n, float f,
    Matrix44f& mat);
void calc_lookat_matrix(
    const Vec3f& pos, const Vec3f& at, const Vec3f& up,
    Matrix44f& mat);

GLuint load_shader(const char* shader_file, GLenum shader_type);
GLuint make_prog(const char* vert_shader_file, const char* frag_shader_file);
GLint get_uniform_loc(GLuint prog, const char* name);

bool check_gl_err();
void check_gl_err_or_die(const char* msg);
