# CLAUDE.md

C++17 library for 3D rotations and coordinate-frame transforms (sensor frame ↔ vehicle frame).
CMake + GoogleTest. No external math libraries.

## Commands
- Configure: `cmake -S . -B build`
- Build: `cmake --build build`
- Test: `ctest --test-dir build --output-on-failure`
- Run demo (once it exists): `./build/rotation_demo`

Always build and run the tests after a change. A step is done only when it builds with zero warnings and all tests pass.

## Layout
- `include/geometry/*.h`: public headers
- `src/*.cpp`: implementation, compiled into the `geometry` library
- `tests/test_*.cpp`: one GoogleTest file per component, all in the `rotation_tests` executable
- `examples/demo.cpp`: `rotation_demo` executable
- When adding a file, also add it to `CMakeLists.txt`. CMake fails if a listed file is missing.

## Code rules
- Simple, readable, sequential code. No one-liners, no clever tricks, no templates, no macros.
- Hand-written math only: no Eigen or other libraries.
- Everything in `namespace geometry`. Headers use `#pragma once`.
- `double` everywhere. Angles in radians.
- Pass `Vec3`/`Mat3` as `const&`. Mark non-mutating methods `const`.
- Comment the math (why), not the syntax (what).
- Keep functions small and single-purpose. Don't add features or files that weren't asked for.

## Ponytail
- Apply the ponytail skill (full level) to every coding task: the smallest change that fully solves the step, no unrequested features, and name what you skipped.
- If the `ponytail` skill can't be invoked, read `~/.claude/plugins/cache/ponytail/ponytail/*/skills/ponytail/SKILL.md` and follow it.
- Where ponytail conflicts with this file (e.g. one-liners), this file wins.

## Conventions (do not change without asking)
- `Mat3` stores `double m[3][3]`, row-major. Vectors are columns: `p' = R * p`.
- Roll/pitch/yaw: `R = Rz(yaw) * Ry(pitch) * Rx(roll)`.
- `Frame` = rotation `R` + translation `t` of a child frame expressed in its parent:
  - `toParent(p) = R * p + t`
  - `toChild(p)  = transpose(R) * (p - t)`
  - `a.compose(b)` means: apply `b` first, then `a`.

## Testing rules
- Write tests together with each component, never at the end.
- Compare floats with `EXPECT_NEAR(..., 1e-9)`. Use small helpers to compare `Vec3`/`Mat3`.
- For rotations, test both known values (90° cases) and properties over a grid of angles:
  `R * transpose(R) == I`, `det(R) == 1`, frame round-trip returns the original point.

## How to work with me
- Go one roadmap step at a time. Stop after each step and wait for me.
- After each step, explain briefly what you wrote: the math and the C++ choices.
- When I say I'll write something myself, review my code and explain problems instead of rewriting it.
- Suggest a commit message at the end of each step. Never commit or push yourself.