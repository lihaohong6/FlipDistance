cmake_minimum_required(VERSION 3.20)
project(FlipDistance)

set(CMAKE_CXX_STANDARD 17)

set(algorithms
        algo/flip_distance.h
        algo/flip_distance_bfs.h algo/flip_distance_source.h)
set(tri triangulation/Helper.cpp triangulation/Helper.h
        triangulation/TriangulatedGraph.cpp triangulation/TriangulatedGraph.h triangulation/BinaryString.cpp 
        triangulation/BinaryString.h triangulation/BinaryTree.h triangulation/Edge.h 
        triangulation/BinaryTree.cpp)
set(rand_utils utils/rand.cpp utils/rand.h)
set(main_program ${algorithms} ${tri})

add_executable(Playground playground.cpp ${main_program})
add_executable(Build main.cpp ${main_program})
target_compile_options(Build PUBLIC -O3)
add_executable(Debug main.cpp ${main_program})
add_executable(RandomTriangulation ${tri} ${rand_utils} rand.cpp)
target_compile_options(RandomTriangulation PUBLIC -O2)

# 'lib' is the folder with Google Test sources
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run ${algorithms} ${tri} ${rand_utils}
        tests/algo/TestFlipDistance.cpp tests/triangulation/TestTriangulationGraph.cpp tests/triangulation/TestBinaryString.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)