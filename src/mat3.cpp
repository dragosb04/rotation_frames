#include "geometry/mat3.h"

namespace geometry {

Mat3::Mat3() {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            m[row][col] = 0.0;
        }
    }
}

Mat3::Mat3(double m00, double m01, double m02,
           double m10, double m11, double m12,
           double m20, double m21, double m22) {
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
}

Mat3 Mat3::identity() {
    return Mat3(1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0);
}

// Rows become columns. For a rotation matrix this is also its inverse.
Mat3 Mat3::transpose() const {
    Mat3 result;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result.m[row][col] = m[col][row];
        }
    }
    return result;
}

// Cofactor expansion along the first row.
// A proper rotation has determinant +1; a reflection would give -1.
double Mat3::determinant() const {
    double minor0 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
    double minor1 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
    double minor2 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
    return m[0][0] * minor0 - m[0][1] * minor1 + m[0][2] * minor2;
}

// Entry (row, col) is the dot product of row `row` of a with column `col` of b.
Mat3 operator*(const Mat3& a, const Mat3& b) {
    Mat3 result;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            double sum = 0.0;
            for (int k = 0; k < 3; k++) {
                sum += a.m[row][k] * b.m[k][col];
            }
            result.m[row][col] = sum;
        }
    }
    return result;
}

// Column-vector convention: each output component is a row of a dotted with v.
Vec3 operator*(const Mat3& a, const Vec3& v) {
    double x = a.m[0][0] * v.x + a.m[0][1] * v.y + a.m[0][2] * v.z;
    double y = a.m[1][0] * v.x + a.m[1][1] * v.y + a.m[1][2] * v.z;
    double z = a.m[2][0] * v.x + a.m[2][1] * v.y + a.m[2][2] * v.z;
    return Vec3(x, y, z);
}

}  // namespace geometry
