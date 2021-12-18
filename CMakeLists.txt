cmake_minimum_required(VERSION 3.20)

project(Cpp_Variant_Experiments)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simplified_rose_tree simplified_rose_tree.cpp)
add_executable(simplified_variant_tree simplified_variant_tree.cpp)
add_executable(shared_ptr_rose_tree shared_ptr_rose_tree.cpp)
