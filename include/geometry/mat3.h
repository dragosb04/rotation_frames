#pragma once

#include "geometry/vec3.h"

namespace geometry {

// 3x3 matrix stored row-major: m[row][col].
class Mat3 {
public:
    double m[3][3];

    // Zero matrix.
    Mat3();

    // Values are given row by row.
    Mat3(double m00, double m01, double m02,
         double m10, double m11, double m12,
         double m20, double m21, double m22);

    static Mat3 identity();

    Mat3 transpose() const;
    double determinant() const;
};

Mat3 operator*(const Mat3& a, const Mat3& b);
Vec3 operator*(const Mat3& a, const Vec3& v);

}  // namespace geometry
