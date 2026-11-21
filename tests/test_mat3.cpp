#include "geometry/mat3.h"

#include <gtest/gtest.h>

using geometry::Mat3;
using geometry::Vec3;

namespace {

const double kTol = 1e-9;

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

// Not symmetric, so swapped rows/columns or a wrong multiplication order shows up.
Mat3 sampleA() {
    return Mat3(1.0, 2.0, 3.0,
                4.0, 5.0, 6.0,
                7.0, 8.0, 10.0);
}

Mat3 sampleB() {
    return Mat3(2.0, 0.0, 1.0,
                1.0, 3.0, 0.0,
                0.0, 1.0, 4.0);
}

}  // namespace

TEST(Mat3Test, DefaultConstructorIsZero) {
    Mat3 zero;
    expectMat3Near(zero, Mat3(0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0,
                              0.0, 0.0, 0.0));
}

TEST(Mat3Test, ConstructorIsRowMajor) {
    Mat3 a = sampleA();
    EXPECT_NEAR(a.m[0][1], 2.0, kTol);
    EXPECT_NEAR(a.m[1][0], 4.0, kTol);
    EXPECT_NEAR(a.m[2][2], 10.0, kTol);
}

TEST(Mat3Test, IdentityHasOnesOnDiagonal) {
    expectMat3Near(Mat3::identity(), Mat3(1.0, 0.0, 0.0,
                                          0.0, 1.0, 0.0,
                                          0.0, 0.0, 1.0));
}

TEST(Mat3Test, Transpose) {
    expectMat3Near(sampleA().transpose(), Mat3(1.0, 4.0, 7.0,
                                               2.0, 5.0, 8.0,
                                               3.0, 6.0, 10.0));
}

TEST(Mat3Test, TransposeTwiceGivesOriginal) {
    expectMat3Near(sampleA().transpose().transpose(), sampleA());
}

TEST(Mat3Test, MultiplyKnownValues) {
    // Row 0 of A times each column of B: 1*2+2*1+3*0, 1*0+2*3+3*1, 1*1+2*0+3*4
    Mat3 expected(4.0, 9.0, 13.0,
                  13.0, 21.0, 28.0,
                  22.0, 34.0, 47.0);
    expectMat3Near(sampleA() * sampleB(), expected);
}

TEST(Mat3Test, MultiplyIsNotCommutative) {
    // B * A row 0: 2*1+0*4+1*7, 2*2+0*5+1*8, 2*3+0*6+1*10
    Mat3 expected(9.0, 12.0, 16.0,
                  13.0, 17.0, 21.0,
                  32.0, 37.0, 46.0);
    expectMat3Near(sampleB() * sampleA(), expected);
}

TEST(Mat3Test, MultiplyByIdentityChangesNothing) {
    expectMat3Near(Mat3::identity() * sampleA(), sampleA());
    expectMat3Near(sampleA() * Mat3::identity(), sampleA());
}

TEST(Mat3Test, MultiplyVectorKnownValues) {
    // 1*1+2*-1+3*2 = 5, 4*1+5*-1+6*2 = 11, 7*1+8*-1+10*2 = 19
    Vec3 v(1.0, -1.0, 2.0);
    expectVec3Near(sampleA() * v, Vec3(5.0, 11.0, 19.0));
}

TEST(Mat3Test, IdentityTimesVectorChangesNothing) {
    Vec3 v(1.0, -1.0, 2.0);
    expectVec3Near(Mat3::identity() * v, v);
}

TEST(Mat3Test, DeterminantKnownValues) {
    EXPECT_NEAR(Mat3::identity().determinant(), 1.0, kTol);
    // 1*(50-48) - 2*(40-42) + 3*(32-35) = 2 + 4 - 9 = -3
    EXPECT_NEAR(sampleA().determinant(), -3.0, kTol);
    // 2*(12-0) - 0*(4-0) + 1*(1-0) = 25
    EXPECT_NEAR(sampleB().determinant(), 25.0, kTol);
}

TEST(Mat3Test, DeterminantOfSingularMatrixIsZero) {
    // Row 2 is row 0 + row 1, so the rows are linearly dependent.
    Mat3 singular(1.0, 2.0, 3.0,
                  4.0, 5.0, 6.0,
                  5.0, 7.0, 9.0);
    EXPECT_NEAR(singular.determinant(), 0.0, kTol);
}

TEST(Mat3Test, DeterminantOfProductIsProductOfDeterminants) {
    double expected = sampleA().determinant() * sampleB().determinant();
    EXPECT_NEAR((sampleA() * sampleB()).determinant(), expected, kTol);
}
