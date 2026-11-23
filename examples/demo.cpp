// Camera mounted on a car: convert a detection from the camera frame to the
// vehicle frame and back.
//
// Vehicle frame (parent): origin on the ground under the rear axle,
//   x forward, y left, z up.
// Camera frame (child): the usual optical convention,
//   x right, y down, z forward (out of the lens).

#include "geometry/frame.h"
#include "geometry/mat3.h"
#include "geometry/vec3.h"

#include <cstdio>

using geometry::Frame;
using geometry::Mat3;
using geometry::Vec3;

namespace {

void printPoint(const char* label, const Vec3& p) {
    std::printf("%-28s (%7.3f, %7.3f, %7.3f)\n", label, p.x, p.y, p.z);
}

}  // namespace

int main() {
    // Each column is a camera axis written in vehicle coordinates:
    //   camera x (right)   -> vehicle -y
    //   camera y (down)    -> vehicle -z
    //   camera z (forward) -> vehicle +x
    Mat3 cameraToVehicle(0.0, 0.0, 1.0,
                         -1.0, 0.0, 0.0,
                         0.0, -1.0, 0.0);

    // Camera sits 1.5 m ahead of the rear axle, centered, 1.4 m above the ground.
    Vec3 cameraPosition(1.5, 0.0, 1.4);

    Frame cameraInVehicle(cameraToVehicle, cameraPosition);

    // A pedestrian's feet seen 10 m ahead of the lens, 1 m to the right,
    // and 1.4 m below it (so on the ground).
    Vec3 detectionInCamera(1.0, 1.4, 10.0);

    Vec3 detectionInVehicle = cameraInVehicle.toParent(detectionInCamera);
    Vec3 backInCamera = cameraInVehicle.toChild(detectionInVehicle);

    printPoint("Detection in camera frame:", detectionInCamera);
    printPoint("Detection in vehicle frame:", detectionInVehicle);
    printPoint("Back in camera frame:", backInCamera);

    // Expected in the vehicle frame: 11.5 m ahead, 1 m to the right (y = -1), on the ground (z = 0).
    return 0;
}
