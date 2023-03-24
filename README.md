# Introduction

This repository implements the algorithm in the [*An 𝒪(3.82^k) Time FPT Algorithm for Convex Flip Distance*](https://drops.dagstuhl.de/opus/volltexte/2023/17696/), which was published in STACS 2023.

# Building and Running

Please make sure you have `cmake` and a C++ compiler installed. We used `Clang 11`, but other compilers should work as well.

Clone or download this repository, and then go to the root directory of this project in the terminal. Run the following commands:

```shell
cmake -S . -B cmake-build-debug
cd cmake-build-debug
cmake --build .
```

The files generated by `cmake` will appear in the `cmake-build-debug` directory. You can run the following command to execute all unit tests. 

```shell
cd cmake-build-debug
./Google_Tests_run
```

# Project Structure

`main.cpp` and `rand.cpp` are entry points to the main program.

The `triangulation` folder contains classes for triangulations, binary trees, and binary strings. The `TriangulatedGraph` class stores triangulations and implements common operations such as flips. The `BinaryTree` class implements a basic binary tree, which is useful when the algorithm needs to convert a triangulation into a binary tree. `BinaryString` is a wrapper class for `vector<bool>` and facilitates conversions between different representations of a triangulation.

The `algo` folder contains 2 algorithms for computing flip distance: a simple breadth-first search algorithm and the FPT algorithm described in the STACS paper. The BFS algorithm is a baseline in performance tests and helps to verify the more complicated algorithm in the paper.

The `tests` folder contains some unit tests.

The `utils` folder contains a utility program that generates random triangulations. 