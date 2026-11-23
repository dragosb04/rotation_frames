#include "geometry/rotation.h"

#include <cmath>

namespace geometry {

// x stays fixed; y and z turn like a 2D rotation in the y-z plane.
Mat3 rotationX(double angle) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    return Mat3(1.0, 0.0, 0.0, 0.0, c, -s, 0.0, s, c);
}

// y stays fixed. The signs look flipped compared to X and Z because
// the cyclic order is z -> x, so z plays the role of "first" axis here.
Mat3 rotationY(double angle) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    return Mat3(c, 0.0, s,
                0.0, 1.0, 0.0,
                -s, 0.0, c);
}

// z stays fixed; x and y turn like a 2D rotation in the x-y plane.
Mat3 rotationZ(double angle) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    return Mat3(c, -s, 0.0,
                s, c, 0.0,
                0.0, 0.0, 1.0);
}

// With column vectors the rightmost matrix acts first, so roll happens first.
Mat3 fromRollPitchYaw(double roll, double pitch, double yaw) {
    return rotationZ(yaw) * rotationY(pitch) * rotationX(roll);
}

// Rodrigues' formula: R = I + sin(a) * K + (1 - cos(a)) * K^2,
// where K is the cross-product matrix of the unit axis (K * v == axis x v).
// Expanded entry by entry: R = c*I + (1-c) * u*u^T + s*K.
Mat3 fromAxisAngle(const Vec3& axis, double angle) {
    Vec3 u = axis.normalized();
    double c = std::cos(angle);
    double s = std::sin(angle);
    double t = 1.0 - c;

    return Mat3(c + t * u.x * u.x, t * u.x * u.y - s * u.z, t * u.x * u.z + s * u.y,
                t * u.y * u.x + s * u.z, c + t * u.y * u.y, t * u.y * u.z - s * u.x,
                t * u.z * u.x - s * u.y, t * u.z * u.y + s * u.x, c + t * u.z * u.z);
}

}  // namespace geometry
