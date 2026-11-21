#include "geometry/vec3.h"

#include <cmath>
#include <stdexcept>

namespace geometry {

Vec3::Vec3(double xValue, double yValue, double zValue) : x(xValue), y(yValue), z(zValue) {}

double Vec3::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vec3 Vec3::normalized() const {
    double length = norm();
    // The zero vector has no direction; dividing would silently produce NaNs.
    if (length == 0.0) {
        throw std::invalid_argument("Vec3::normalized: cannot normalize the zero vector");
    }
    return Vec3(x / length, y / length, z / length);
}

Vec3 operator+(const Vec3& a, const Vec3& b) {
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

}  // namespace geometry
