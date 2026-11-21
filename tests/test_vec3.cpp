#include "geometry/vec3.h"

#include <gtest/gtest.h>

#include <stdexcept>

namespace geometry {
    class Vec3;
}

using geometry::Vec3;

namespace {

const double kTol = 1e-9;

void expectVec3Near(const Vec3& actual, const Vec3& expected) {
    EXPECT_NEAR(actual.x, expected.x, kTol);
    EXPECT_NEAR(actual.y, expected.y, kTol);
    EXPECT_NEAR(actual.z, expected.z, kTol);
}

}  // namespace

TEST(Vec3Test, Addition) {
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(4.0, -5.0, 6.0);
    expectVec3Near(a + b, Vec3(5.0, -3.0, 9.0));
}

TEST(Vec3Test, Subtraction) {
    Vec3 a(1.0, 2.0, 3.0);
    Vec3 b(4.0, -5.0, 6.0);
    expectVec3Near(a - b, Vec3(-3.0, 7.0, -3.0));
}

TEST(Vec3Test, Norm) {
    // sqrt(9 + 16 + 144) = 13
    Vec3 v(3.0, 4.0, 12.0);
    EXPECT_NEAR(v.norm(), 13.0, kTol);
}

TEST(Vec3Test, NormalizedHasUnitLengthAndSameDirection) {
    Vec3 v(3.0, 4.0, 12.0);
    Vec3 n = v.normalized();
    EXPECT_NEAR(n.norm(), 1.0, kTol);
    expectVec3Near(n, Vec3(3.0 / 13.0, 4.0 / 13.0, 12.0 / 13.0));
}

TEST(Vec3Test, NormalizingZeroVectorThrows) {
    Vec3 zero(0.0, 0.0, 0.0);
    EXPECT_THROW(zero.normalized(), std::invalid_argument);
}
