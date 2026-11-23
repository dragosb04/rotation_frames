#include "geometry/frame.h"
#include "geometry/rotation.h"

#include <vector>

#include <gtest/gtest.h>

using geometry::Frame;
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

void expectFrameNear(const Frame& actual, const Frame& expected) {
    expectMat3Near(actual.rotation, expected.rotation);
    expectVec3Near(actual.translation, expected.translation);
}

// Angles from -pi to pi in steps of pi/6.
std::vector<double> angleGrid() {
    std::vector<double> angles;
    for (int i = -6; i <= 6; i++) {
        angles.push_back(i * kPi / 6.0);
    }
    return angles;
}

Frame identityFrame() {
    return Frame(Mat3::identity(), Vec3(0.0, 0.0, 0.0));
}

// Child turned 90° about z and placed at (1, 2, 3) in the parent.
Frame yawedFrame() {
    return Frame(geometry::rotationZ(kHalfPi), Vec3(1.0, 2.0, 3.0));
}

// A general frame with no special angles, so sign or order mistakes show up.
Frame generalFrame() {
    return Frame(geometry::fromRollPitchYaw(0.3, -0.5, 1.1), Vec3(-2.0, 0.5, 4.0));
}

const Vec3 kPoint(0.7, -1.4, 2.5);

}  // namespace

TEST(FrameTest, ToParentKnownValues) {
    // Child x axis points along parent y, so child (1,0,0) lands at origin + (0,1,0).
    expectVec3Near(yawedFrame().toParent(Vec3(1.0, 0.0, 0.0)), Vec3(1.0, 3.0, 3.0));
    // The child origin is the translation.
    expectVec3Near(yawedFrame().toParent(Vec3(0.0, 0.0, 0.0)), Vec3(1.0, 2.0, 3.0));
}

TEST(FrameTest, ToChildKnownValues) {
    expectVec3Near(yawedFrame().toChild(Vec3(1.0, 3.0, 3.0)), Vec3(1.0, 0.0, 0.0));
    // Parent origin is at (-1,-2,-3) relative to the child origin, then rotated by -90° about z.
    expectVec3Near(yawedFrame().toChild(Vec3(0.0, 0.0, 0.0)), Vec3(-2.0, 1.0, -3.0));
}

TEST(FrameTest, IdentityFrameChangesNothing) {
    expectVec3Near(identityFrame().toParent(kPoint), kPoint);
    expectVec3Near(identityFrame().toChild(kPoint), kPoint);
}

TEST(FrameTest, RoundTripReturnsOriginalPointOverAngleGrid) {
    Vec3 translation(-2.0, 0.5, 4.0);
    for (double roll : angleGrid()) {
        for (double pitch : angleGrid()) {
            for (double yaw : angleGrid()) {
                Frame frame(geometry::fromRollPitchYaw(roll, pitch, yaw), translation);
                expectVec3Near(frame.toChild(frame.toParent(kPoint)), kPoint);
                expectVec3Near(frame.toParent(frame.toChild(kPoint)), kPoint);
            }
        }
    }
}

TEST(FrameTest, InverseToParentEqualsToChild) {
    Frame frame = generalFrame();
    expectVec3Near(frame.inverse().toParent(kPoint), frame.toChild(kPoint));
    expectVec3Near(frame.inverse().toChild(kPoint), frame.toParent(kPoint));
}

TEST(FrameTest, InverseKnownValues) {
    Frame expected(geometry::rotationZ(-kHalfPi), Vec3(-2.0, 1.0, -3.0));
    expectFrameNear(yawedFrame().inverse(), expected);
}

TEST(FrameTest, InverseTwiceGivesOriginal) {
    expectFrameNear(generalFrame().inverse().inverse(), generalFrame());
}

TEST(FrameTest, ComposeWithInverseIsIdentity) {
    expectFrameNear(generalFrame().compose(generalFrame().inverse()), identityFrame());
    expectFrameNear(generalFrame().inverse().compose(generalFrame()), identityFrame());
}

TEST(FrameTest, ComposeWithIdentityChangesNothing) {
    expectFrameNear(generalFrame().compose(identityFrame()), generalFrame());
    expectFrameNear(identityFrame().compose(generalFrame()), generalFrame());
}

TEST(FrameTest, ComposeAppliesOtherFirst) {
    Frame a = yawedFrame();
    Frame b = generalFrame();
    expectVec3Near(a.compose(b).toParent(kPoint), a.toParent(b.toParent(kPoint)));
}

TEST(FrameTest, ComposeKnownValues) {
    // b: pure shift by (1,0,0). a: yaw 90° then shift by (0,0,5).
    // b moves the origin to (1,0,0); a turns that into (0,1,0) and lifts it to (0,1,5).
    Frame a(geometry::rotationZ(kHalfPi), Vec3(0.0, 0.0, 5.0));
    Frame b(Mat3::identity(), Vec3(1.0, 0.0, 0.0));
    Frame expected(geometry::rotationZ(kHalfPi), Vec3(0.0, 1.0, 5.0));
    expectFrameNear(a.compose(b), expected);
}

TEST(FrameTest, ComposeIsNotCommutative) {
    Frame a(geometry::rotationZ(kHalfPi), Vec3(0.0, 0.0, 5.0));
    Frame b(Mat3::identity(), Vec3(1.0, 0.0, 0.0));
    // b after a: the shift (1,0,0) is not rotated, so the origin goes to (1,0,5).
    Frame expected(geometry::rotationZ(kHalfPi), Vec3(1.0, 0.0, 5.0));
    expectFrameNear(b.compose(a), expected);
}
