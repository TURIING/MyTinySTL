cmake_minimum_required(VERSION 3.20)
project(MyTinySTL)

set(CMAKE_CXX_STANDARD 20)

add_executable(MyTinySTL main.cpp Allocator/MallocAlloc.cpp Allocator/MallocAlloc.h Allocator/DefaultAlloc.cpp Allocator/DefaultAlloc.h)
