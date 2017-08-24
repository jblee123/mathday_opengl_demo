#pragma once

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

struct Vec4f {

    float x;
    float y;
    float z;
    float w;

    Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
    }

    Vec4f() : Vec4f(0, 0, 0, 0) {
    }

    Vec4f(const Vec4f& other) : Vec4f(other.x, other.y, other.z, other.w) {
    }

    Vec4f& operator=(const Vec4f& rhs) {
        if (this != &rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
        }
        return *this;
    }

    float operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;

        assert(false);
        return 0;
    }

    float& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;

        assert(false);
        return x;
    }

    Vec4f operator+(const Vec4f& rhs) const {
        Vec4f result = {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
            w + rhs.w
        };
        return result;
    }

    Vec4f operator-(const Vec4f& rhs) const {
        Vec4f result = {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z,
            w - rhs.w
        };
        return result;
    }

    Vec4f operator*(float scalar) const {
        Vec4f result = {
            x * scalar,
            y * scalar,
            z * scalar,
            w * scalar
        };
        return result;
    }

    Vec4f operator/(float scalar) const {
        Vec4f result = {
            x / scalar,
            y / scalar,
            z / scalar,
            w / scalar
        };
        return result;
    }

    Vec4f& operator+=(const Vec4f& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vec4f& operator-=(const Vec4f& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vec4f& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Vec4f& operator/=(double scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    float dot(const Vec4f& rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }

    float length() const {
        return sqrtf(dot(*this));
    }

    Vec4f norm() const {
        float len = length();
        return *this / len;
    }

    // Vec4f cross(const Vec4f& rhs) const {
    //     Vec4f result(
    //         y * rhs.z - z * rhs.y,
    //         z * rhs.x - x * rhs.z,
    //         x * rhs.y - y * rhs.x,
    //         x * rhs.y - y * rhs.x
    //     );
    //     return result;
    // }
};

struct Vec4i {

    int x;
    int y;
    int z;
    int w;

    Vec4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {
    }

    Vec4i() : Vec4i(0, 0, 0, 0) {
    }

    Vec4i(const Vec4i& other) : Vec4i(other.x, other.y, other.z, other.w) {
    }

    Vec4i& operator=(const Vec4i& rhs) {
        if (this != &rhs) {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = rhs.w;
        }
        return *this;
    }

    int operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;

        assert(false);
        return 0;
    }

    int& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        if (i == 2) return z;
        if (i == 3) return w;

        assert(false);
        return x;
    }

    Vec4i operator+(const Vec4i& rhs) const {
        Vec4i result = {
            x + rhs.x,
            y + rhs.y,
            z + rhs.z,
            w + rhs.w
        };
        return result;
    }

    Vec4i operator-(const Vec4i& rhs) const {
        Vec4i result = {
            x - rhs.x,
            y - rhs.y,
            z - rhs.z,
            w - rhs.w
        };
        return result;
    }

    Vec4i operator*(int scalar) const {
        Vec4i result = {
            x * scalar,
            y * scalar,
            z * scalar,
            w * scalar
        };
        return result;
    }

    Vec4i operator/(int scalar) const {
        Vec4i result = {
            x / scalar,
            y / scalar,
            z / scalar,
            w / scalar
        };
        return result;
    }

    Vec4i& operator+=(const Vec4i& rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vec4i& operator-=(const Vec4i& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vec4i& operator*=(int scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    Vec4i& operator/=(int scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    // int dot(const Vec4i& rhs) const {
    //     return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    // }

    // int length() const {
    //     return sqrtf(dot(*this));
    // }

    // Vec4i norm() const {
    //     float len = length();
    //     return *this / len;
    // }

    // Vec4i cross(const Vec4i& rhs) const {
    //     Vec4i result(
    //         y * rhs.z - z * rhs.y,
    //         z * rhs.x - x * rhs.z,
    //         x * rhs.y - y * rhs.x,
    //         x * rhs.y - y * rhs.x
    //     );
    //     return result;
    // }
};
