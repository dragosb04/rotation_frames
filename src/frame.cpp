#include "geometry/frame.h"

namespace geometry {

Frame::Frame(const Mat3& rotationValue, const Vec3& translationValue)
    : rotation(rotationValue), translation(translationValue) {}

// Rotate into parent axes, then shift by where the child origin sits in the parent.
Vec3 Frame::toParent(const Vec3& p) const {
    return rotation * p + translation;
}

// Undo toParent in reverse order: remove the shift, then rotate back.
// A rotation's inverse is its transpose, so no general matrix inverse is needed.
Vec3 Frame::toChild(const Vec3& p) const {
    return rotation.transpose() * (p - translation);
}

// Solving p = R * c + t for c gives c = R^T * p - R^T * t.
// The new translation -R^T * t is exactly toChild of the parent origin.
Frame Frame::inverse() const {
    Vec3 parentOrigin(0.0, 0.0, 0.0);
    return Frame(rotation.transpose(), toChild(parentOrigin));
}

// a.toParent(b.toParent(p)) = Ra * (Rb * p + tb) + ta = (Ra * Rb) * p + (Ra * tb + ta).
// The new translation Ra * tb + ta is this->toParent(tb).
Frame Frame::compose(const Frame& other) const {
    return Frame(rotation * other.rotation, toParent(other.translation));
}

}  // namespace geometry
