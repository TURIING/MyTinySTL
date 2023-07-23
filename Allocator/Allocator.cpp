//
// Created by TURIING on 2023/7/19.
//

#include "Allocator.h"

namespace STL {
    /***************************************************** allocate **********************************************/
    template<class T, class Alloc>
    T *Allocator<T, Alloc>::allocate() {
        return reinterpret_cast<T *>(Alloc::allocate(sizeof(T)));
    }

    template<class T, class Alloc>
    T *Allocator<T, Alloc>::allocate(size_t _n) {
        assert(_n > 0);

        return reinterpret_cast<T *>(Alloc::allocate(sizeof(T) * _n));
    }

    /***************************************************** deallocate **********************************************/
    template<class T, class Alloc>
    void Allocator<T, Alloc>::deallocate(T *_ptr) {
        assert(_ptr != nullptr);
        Alloc::deallocate(reinterpret_cast<void *>(_ptr), sizeof(T));
    }

    template<class T, class Alloc>
    void Allocator<T, Alloc>::deallocate(T *_ptr, size_t _n) {
        assert(_ptr != nullptr);
        assert(_n > 0);

        Alloc::deallocate(reinterpret_cast<void *>(_ptr), sizeof(T) * _n);
    }

    /***************************************************** construct **********************************************/
    template<class T, class Alloc>
    void Allocator<T, Alloc>::construct(T *_ptr) {
        assert(_ptr != nullptr);

        // placement new
        new (_ptr) T();
    }

    template<class T, class Alloc>
    void Allocator<T, Alloc>::construct(T *_ptr, const T &_value) {
        assert(_ptr != nullptr);

        new (_ptr) T(_value);
    }

    /***************************************************** destroy **********************************************/
    template<class T, class Alloc>
    void Allocator<T, Alloc>::destroy(T *_ptr) {
        assert(_ptr != nullptr);

        _ptr->~T();
    }

    template<class T, class Alloc>
    void Allocator<T, Alloc>::destroy(T *_first, T *_last) {
        assert(_first != nullptr);
        assert(_last != nullptr);

        for(; _first != _last; ++_first)
            _first->~T();
    }
}