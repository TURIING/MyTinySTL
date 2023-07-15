//
// Created by TURIING on 2023/7/14.
//

#ifndef MYTINYSTL_MALLOCALLOC_H
#define MYTINYSTL_MALLOCALLOC_H

#include <cstddef>
#include <stdlib.h>
#include <new>

namespace STL {
    /*
     * 一级空间分配器
     */
    class MallocAlloc {
    public:
        using malloc_handler = void (*)();
    private:
        static malloc_handler m_malloc_alloc_handler;
    private:
        static void *m_malloc(size_t _n);
        static void *m_realloc(void *_p, size_t _n);

    public:
        static void *allocate(size_t _n) {
            void *result = malloc(_n);
            if(result == nullptr) result = m_malloc(_n);
            return result;
        }

        static void *reallocate(void *_p, size_t, size_t _new_size) {
            void *result = realloc(_p, _new_size);
            if(result == nullptr) m_realloc(_p, _new_size);
            return result;
        }

        static malloc_handler setMallocHandler(malloc_handler _func) {
            malloc_handler old = m_malloc_alloc_handler;
            m_malloc_alloc_handler = _func;
            return old;
        }

        static void deallocate(void *_p, size_t) { free(_p); };
    };
}


#endif //MYTINYSTL_MALLOCALLOC_H
