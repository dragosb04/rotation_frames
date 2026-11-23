# rotation-frames

A small C++17 library for 3D rotations and coordinate-frame transforms, such as converting
a point from a camera (sensor) frame to the vehicle frame and back.

All math is written by hand, with no Eigen or other math libraries. The code is kept simple and
commented so it can be read as a learning reference.

## Features

- `Vec3`: 3D vector with `+`, `-`, `norm()`, `normalized()`
- `Mat3`: 3x3 matrix with matrix-matrix and matrix-vector multiply, `transpose()`, `determinant()`
- Rotations: `rotationX/Y/Z`, `fromRollPitchYaw`, `fromAxisAngle` (Rodrigues' formula)
- `Frame`: rigid transform (rotation + translation) with `toParent`, `toChild`, `inverse`, `compose`

## Conventions

- Angles are in radians, and all values are `double`.
- `Mat3` is stored row-major (`m[row][col]`). Vectors are columns: `p' = R * p`.
- Rotations are right-handed. A positive angle turns counter-clockwise when looking down the axis toward the origin.
- Roll/pitch/yaw: `R = Rz(yaw) * Ry(pitch) * Rx(roll)`, so roll is applied first and yaw last.
- A `Frame` is the pose of a child frame expressed in its parent:
  - `toParent(p) = R * p + t`
  - `toChild(p)  = transpose(R) * (p - t)`
  - `a.compose(b)` applies `b` first, then `a`. If `b` is C in B and `a` is B in A, the result is C in A.

## Build and test

Requires CMake 3.20+ and a C++17 compiler. GoogleTest is downloaded automatically at configure time.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Demo

`examples/demo.cpp` places a camera on a car and converts a detected point from the camera frame
to the vehicle frame and back.

```sh
./build/rotation_demo
```

```
Detection in camera frame:   (  1.000,   1.400,  10.000)
Detection in vehicle frame:  ( 11.500,  -1.000,   0.000)
Back in camera frame:        (  1.000,   1.400,  10.000)
```

The vehicle frame has x forward, y left and z up. The camera uses the usual camera convention of
x right, y down and z forward. The camera is mounted 1.5 m ahead of the rear axle and 1.4 m up,
so a point 10 m in front of the lens and 1.4 m below it lands on the ground (z = 0) 11.5 m ahead
of the car.

## Usage

```cpp
#include "geometry/frame.h"
#include "geometry/rotation.h"

using namespace geometry;

// Sensor turned 90° to the left (yaw) and mounted 2 m forward, 1 m up.
Frame sensorInVehicle(fromRollPitchYaw(0.0, 0.0, 1.5707963267948966), Vec3(2.0, 0.0, 1.0));

Vec3 inVehicle = sensorInVehicle.toParent(Vec3(5.0, 0.0, 0.0));  // (2, 5, 1)
Vec3 inSensor  = sensorInVehicle.toChild(inVehicle);              // (5, 0, 0)
```

## Layout

```
include/geometry/   public headers (vec3.h, mat3.h, rotation.h, frame.h)
src/                implementation, built into the `geometry` library
tests/              GoogleTest suites, built into `rotation_tests`
examples/demo.cpp   the `rotation_demo` executable
```

## Testing approach

- Known values: 90° rotations about each axis and hand-computed frame transforms.
- Properties over a grid of angles: `R * transpose(R) == I`, `det(R) == 1`, and a frame
  round trip returns the original point.
