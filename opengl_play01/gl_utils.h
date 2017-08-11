#pragma once

#include <GL/glew.h>

#define PI 3.141592653589793
#define PI_F ((float)3.141592653589793)

GLuint make_prog(const char* vert_shader_file, const char* frag_shader_file);
bool check_gl_err();
void check_gl_err_or_die(const char* msg);
