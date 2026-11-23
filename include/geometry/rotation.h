#pragma once

#include "geometry/mat3.h"
#include "geometry/vec3.h"

namespace geometry {

// Right-handed rotations by `angle` radians about a coordinate axis.
// Positive angles turn counter-clockwise when looking down the axis toward the origin.
Mat3 rotationX(double angle);
Mat3 rotationY(double angle);
Mat3 rotationZ(double angle);

// R = Rz(yaw) * Ry(pitch) * Rx(roll): roll is applied first, yaw last.
Mat3 fromRollPitchYaw(double roll, double pitch, double yaw);

// Rotation by `angle` about `axis`. The axis does not need to be unit length.
// Throws std::invalid_argument for the zero vector.
Mat3 fromAxisAngle(const Vec3& axis, double angle);

}  // namespace geometry
