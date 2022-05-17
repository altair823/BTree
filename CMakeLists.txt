cmake_minimum_required(VERSION 3.22)
project(b_tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(b_tree main.cpp b_tree.h node/node.h data.h result.h b_tree.tpp)

add_subdirectory(tests)