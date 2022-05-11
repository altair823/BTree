cmake_minimum_required(VERSION 3.22)
project(b_tree)

set(CMAKE_CXX_STANDARD 20)

add_executable(b_tree main.cpp b_tree.h node/node.h data.h result.h)

add_subdirectory(tests)