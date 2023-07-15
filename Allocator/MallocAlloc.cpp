//
// Created by TURIING on 2023/7/14.
//

#include "MallocAlloc.h"

namespace STL {
    typename MallocAlloc::malloc_handler MallocAlloc::m_malloc_alloc_handler = nullptr;

    void *MallocAlloc::m_malloc(size_t _n) {
        malloc_handler new_alloc_handler;
        void *result;
        while (true) {
            new_alloc_handler = m_malloc_alloc_handler;
            if(!new_alloc_handler) throw std::bad_alloc();
            //调用handler，试图释放内存
            (*new_alloc_handler)();
            result = malloc(_n);
            if(result) return result;
        }
    }

    void *MallocAlloc::m_realloc(void *_p, size_t _n) {
        malloc_handler new_alloc_handler;
        void *result;
        while(true) {
            new_alloc_handler = m_malloc_alloc_handler;
            if(!new_alloc_handler) throw std::bad_alloc();
            (*new_alloc_handler)();
            result = realloc(_p, _n);
            if(result) return result;
        }
    }
}


