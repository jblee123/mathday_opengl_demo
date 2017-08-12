#pragma once

#include <cassert>
#include <cmath>

#include <GL/glew.h>

#define PI 3.141592653589793
#define PI_F ((float)3.141592653589793)

struct Vec3f {

    float x;
    float y;
    float z;

    Vec3f(float x, float y, float z) : x(x), y(y), z(z) {
    }

    Vec3f() : Vec3f(0, 0, 0) {
    }

    Vec3f(const Vec3f& other) : Vec3f(other.x, other.y, other.z) {
    }

    Vec3f& operator=(const Vec3f& rhs) {
        if (this != &rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
        }
        return *this;
    }

    float operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;

        assert(false);
        return 0;
    }

    float& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;

        assert(false);
        return x;
    }

    Vec3f operator+(const Vec3f& rhs) const {
        Vec3f result = {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z
        };
        return result;
    }

    Vec3f operator-(const Vec3f& rhs) const {
        Vec3f result = {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z
        };
        return result;
    }

    Vec3f operator*(float scalar) const {
        Vec3f result = {
            x * scalar,
            y * scalar,
            z * scalar
        };
        return result;
    }

    Vec3f operator/(float scalar) const {
        Vec3f result = {
            x / scalar,
            y / scalar,
            z / scalar
        };
        return result;
    }

    Vec3f& operator+=(const Vec3f& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3f& operator-=(const Vec3f& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3f& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3f& operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    float dot(const Vec3f& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }

    float length() const {
        return sqrtf(dot(*this));
    }

    Vec3f norm() const {
        float len = length();
        return *this / len;
    }

    Vec3f cross(const Vec3f& rhs) const {
        Vec3f result(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
        return result;
    }
};

typedef float Matrix44f[4][4];

void get_perspective_info(
    float fov, float aspect_ratio, float near, float far,
    float& b, float& t, float& l, float& r);
void calc_proj_matrix(
    float b, float t, float l, float r, float n, float f,
    Matrix44f& mat);
void calc_lookat_matrix(
    const Vec3f& pos, const Vec3f& at, const Vec3f& up,
    Matrix44f& mat);

GLuint make_prog(const char* vert_shader_file, const char* frag_shader_file);
GLint get_uniform_loc(GLuint prog, const char* name);

bool check_gl_err();
void check_gl_err_or_die(const char* msg);
