#pragma once

#include "geometry/mat3.h"
#include "geometry/vec3.h"

namespace geometry {

class Frame {
public:
    Mat3 rotation;
    Vec3 translation;

    Frame(const Mat3& rotationValue, const Vec3& translationValue);

    Vec3 toParent(const Vec3& p) const;

    Vec3 toChild(const Vec3& p) const;

    Frame inverse() const;

    // Apply `other` first, then this frame.
    // If `other` is C in B and this is B in A, the result is C in A.
    Frame compose(const Frame& other) const;
};

}  // namespace geometry
