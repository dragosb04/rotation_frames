#pragma once

namespace geometry {

class Vec3 {
public:
    double x;
    double y;
    double z;

    Vec3(double xValue, double yValue, double zValue);

    double norm() const;

    // Throws std::invalid_argument for the zero vector.
    Vec3 normalized() const;
};

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);

}  // namespace geometry
