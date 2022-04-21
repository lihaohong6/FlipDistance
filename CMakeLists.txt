cmake_minimum_required(VERSION 3.20)
project(FlipDistance)

set(CMAKE_CXX_STANDARD 17)

add_executable(FlipDistance main.cpp triangulation/TriangulatedGraph.cpp triangulation/TriangulatedGraph.h triangulation/BinaryString.cpp triangulation/BinaryString.h utils/helper.h algo/flip_distance.h)

# 'lib' is the folder with Google Test sources
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run tests/triangulation/TestTriangulationGraph.cpp triangulation/TriangulatedGraph.cpp triangulation/TriangulatedGraph.h triangulation/BinaryString.cpp triangulation/BinaryString.h utils/helper.h tests/triangulation/TestTriangulationGraph.cpp algo/flip_distance.h tests/algo/TestFlipDistance.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)