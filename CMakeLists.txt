cmake_minimum_required(VERSION 3.20)
project(MyTinySTL)

set(CMAKE_CXX_STANDARD 20)
include_directories(${CMAKE_SOURCE_DIR})

add_executable(MyTinySTL main.cpp Allocator/MallocAlloc.cpp Allocator/MallocAlloc.h Allocator/DefaultAlloc.cpp Allocator/DefaultAlloc.h Allocator/Allocator.cpp Allocator/Allocator.h Containers/Vector/Vector.cpp Containers/Vector/Vector.h Iterator/ReverseIterator.cpp Iterator/ReverseIterator.h Allocator/uninitialized.h Allocator/type_traits.h Iterator/Iterator.h Algorithm/algobase/algobase.h Allocator/construct.h)
