cmake_minimum_required(VERSION 3.14)
project(rotation_frames CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)

# Library
add_library(geometry
    src/vec3.cpp
    src/mat3.cpp
    src/rotation.cpp
    src/frame.cpp
        include/geometry/rotation.h
)
target_include_directories(geometry PUBLIC include)

# GoogleTest is not installed system-wide, so download a pinned release at configure time.
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Tests
enable_testing()
add_executable(rotation_tests
    tests/test_vec3.cpp
    tests/test_mat3.cpp
    tests/test_rotation.cpp
    tests/test_frame.cpp
)
target_link_libraries(rotation_tests PRIVATE geometry GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(rotation_tests)

# Demo
add_executable(rotation_demo examples/demo.cpp)
target_link_libraries(rotation_demo PRIVATE geometry)
