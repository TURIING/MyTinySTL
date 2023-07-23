//
// Created by TURIING on 2023/7/19.
//

#ifndef MYTINYSTL_ALLOCATOR_H
#define MYTINYSTL_ALLOCATOR_H

#include "DefaultAlloc.h"
#include <cstddef>
#include <cassert>

namespace STL {
    template<class T, class Alloc = DefaultAlloc>
    class Allocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

    public:
        static T *allocate();
        static T *allocate(size_t _n);

        static void deallocate(T *_ptr);
        static void deallocate(T *_ptr, size_t _n);

        static void construct(T *_ptr);
        static void construct(T *_ptr, const T &_value);

        static void destroy(T *_ptr);
        static void destroy(T *_first, T *_last);
    };




}


#endif //MYTINYSTL_ALLOCATOR_H
