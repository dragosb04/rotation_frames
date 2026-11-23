#include "geometry/rotation.h"

#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

using geometry::Mat3;
using geometry::Vec3;

namespace {

const double kTol = 1e-9;
const double kPi = 3.14159265358979323846;
const double kHalfPi = kPi / 2.0;

void expectMat3Near(const Mat3& actual, const Mat3& expected) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            EXPECT_NEAR(actual.m[row][col], expected.m[row][col], kTol)
                << "at row " << row << ", col " << col;
        }
    }
}

void expectVec3Near(const Vec3& actual, const Vec3& expected) {
    EXPECT_NEAR(actual.x, expected.x, kTol);
    EXPECT_NEAR(actual.y, expected.y, kTol);
    EXPECT_NEAR(actual.z, expected.z, kTol);
}

// A proper rotation is orthonormal (R * R^T == I) and keeps handedness (det == +1).
void expectIsRotation(const Mat3& r) {
    expectMat3Near(r * r.transpose(), Mat3::identity());
    EXPECT_NEAR(r.determinant(), 1.0, kTol);
}

// Angles from -pi to pi in steps of pi/6, including 0 and the 90° cases.
std::vector<double> angleGrid() {
    std::vector<double> angles;
    for (int i = -6; i <= 6; i++) {
        angles.push_back(i * kPi / 6.0);
    }
    return angles;
}

const Vec3 kX(1.0, 0.0, 0.0);
const Vec3 kY(0.0, 1.0, 0.0);
const Vec3 kZ(0.0, 0.0, 1.0);

}  // namespace

// Right-hand rule, +90°: x -> y, y -> z, z -> x (cyclic).

TEST(RotationTest, RotationXBy90DegreesTurnsYIntoZ) {
    Mat3 r = geometry::rotationX(kHalfPi);
    expectVec3Near(r * kY, kZ);
    expectVec3Near(r * kZ, Vec3(0.0, -1.0, 0.0));
    expectVec3Near(r * kX, kX);
}

TEST(RotationTest, RotationYBy90DegreesTurnsZIntoX) {
    Mat3 r = geometry::rotationY(kHalfPi);
    expectVec3Near(r * kZ, kX);
    expectVec3Near(r * kX, Vec3(0.0, 0.0, -1.0));
    expectVec3Near(r * kY, kY);
}

TEST(RotationTest, RotationZBy90DegreesTurnsXIntoY) {
    Mat3 r = geometry::rotationZ(kHalfPi);
    expectVec3Near(r * kX, kY);
    expectVec3Near(r * kY, Vec3(-1.0, 0.0, 0.0));
    expectVec3Near(r * kZ, kZ);
}

TEST(RotationTest, ZeroAngleIsIdentity) {
    expectMat3Near(geometry::rotationX(0.0), Mat3::identity());
    expectMat3Near(geometry::rotationY(0.0), Mat3::identity());
    expectMat3Near(geometry::rotationZ(0.0), Mat3::identity());
    expectMat3Near(geometry::fromRollPitchYaw(0.0, 0.0, 0.0), Mat3::identity());
    expectMat3Near(geometry::fromAxisAngle(Vec3(1.0, 2.0, 3.0), 0.0), Mat3::identity());
}

TEST(RotationTest, RollPitchYawAppliesRollFirst) {
    // Roll 90° sends y to z, then yaw 90° leaves z alone. Pitch is zero.
    // If the order were reversed, yaw would first send y to -x and roll would keep it there.
    Mat3 r = geometry::fromRollPitchYaw(kHalfPi, 0.0, kHalfPi);
    expectVec3Near(r * kY, kZ);
}

TEST(RotationTest, RollPitchYawMatchesProductOfAxisRotations) {
    double roll = 0.1;
    double pitch = -0.4;
    double yaw = 1.3;
    Mat3 expected = geometry::rotationZ(yaw) * geometry::rotationY(pitch) * geometry::rotationX(roll);
    expectMat3Near(geometry::fromRollPitchYaw(roll, pitch, yaw), expected);
}

TEST(RotationTest, AxisAngleAboutCoordinateAxesMatchesAxisRotations) {
    for (double angle : angleGrid()) {
        expectMat3Near(geometry::fromAxisAngle(kX, angle), geometry::rotationX(angle));
        expectMat3Near(geometry::fromAxisAngle(kY, angle), geometry::rotationY(angle));
        expectMat3Near(geometry::fromAxisAngle(kZ, angle), geometry::rotationZ(angle));
    }
}

TEST(RotationTest, AxisAngleNormalizesTheAxis) {
    expectMat3Near(geometry::fromAxisAngle(Vec3(0.0, 0.0, 5.0), 0.7), geometry::rotationZ(0.7));
}

TEST(RotationTest, AxisAngleKeepsTheAxisFixed) {
    Vec3 axis(1.0, 1.0, 1.0);
    expectVec3Near(geometry::fromAxisAngle(axis, 0.9) * axis, axis);
}

TEST(RotationTest, AxisAngle120DegreesAboutDiagonalCyclesAxes) {
    // A third of a turn about (1,1,1) permutes the axes: x -> y -> z -> x.
    Mat3 r = geometry::fromAxisAngle(Vec3(1.0, 1.0, 1.0), 2.0 * kPi / 3.0);
    expectVec3Near(r * kX, kY);
    expectVec3Near(r * kY, kZ);
    expectVec3Near(r * kZ, kX);
}

TEST(RotationTest, AxisAngleZeroAxisThrows) {
    EXPECT_THROW(geometry::fromAxisAngle(Vec3(0.0, 0.0, 0.0), 1.0), std::invalid_argument);
}

TEST(RotationTest, AxisRotationsAreProperRotationsOverAngleGrid) {
    for (double angle : angleGrid()) {
        expectIsRotation(geometry::rotationX(angle));
        expectIsRotation(geometry::rotationY(angle));
        expectIsRotation(geometry::rotationZ(angle));
    }
}

TEST(RotationTest, RollPitchYawIsProperRotationOverAngleGrid) {
    for (double roll : angleGrid()) {
        for (double pitch : angleGrid()) {
            for (double yaw : angleGrid()) {
                expectIsRotation(geometry::fromRollPitchYaw(roll, pitch, yaw));
            }
        }
    }
}

TEST(RotationTest, AxisAngleIsProperRotationOverAngleGrid) {
    Vec3 axis(0.3, -1.2, 0.8);
    for (double angle : angleGrid()) {
        expectIsRotation(geometry::fromAxisAngle(axis, angle));
    }
}
