//
// Created by TURIING on 2023/7/20.
//

#ifndef MYTINYSTL_UNINITIALIZED_H
#define MYTINYSTL_UNINITIALIZED_H


#include "Allocator/type_traits.h"
#include "Allocator/construct.h"
#include "Iterator/Iterator.h"
#include "Algorithm/algobase/algobase.h"

namespace STL {
    /******************************************** uninitialized_fill ************************************************/
    template<class ForwardIterator, class T>
    inline void uninitialized_fill_aux(ForwardIterator _first, ForwardIterator _last, const T &_value, true_type) {
        STL::fill(_first, _last, _value);
    }

    template<class ForwardIterator, class T>
    inline void uninitialized_fill_aux(ForwardIterator _first, ForwardIterator _last, const T &_value, false_type) {
        auto cur = _first;
        for(; cur != _last; ++cur)
            STL::construct(&*cur, _value);

    }

    template<class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator _first, ForwardIterator _last, const T &_value) {
        using isPodType = typename type_traits<value_type_t<ForwardIterator>>::is_POD_type;
        uninitialized_fill_aux(_first, _last, _value, isPodType());
    }

    /******************************************** uninitialized_fill_n ************************************************/
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator _first, Size _n, const T &_value, true_type) {
        return STL::fill_n(_first, _n, _value);
    }

    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n_aux(ForwardIterator _first, Size _n, const T &_value, false_type) {
        auto cur = _first;
        for(; _n > 0; --_n, ++cur)
            STL::construct(&*cur, _value);
        return cur;
    }

    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator _first, Size _n, const T &_value) {
        using isPodType = typename type_traits<value_type_t<ForwardIterator>>::is_POD_type;
        return uninitialized_fill_n_aux(_first, _n, _value, isPodType());
    }

    /******************************************** uninitialized_copy ************************************************/
    /* 如果为POD类型，则直接拷贝 */
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy_aux(InputIterator _first, InputIterator _last, ForwardIterator _result, true_type) {
        return STL::copy(_first, _last, _result);
    }
    /* 如果不是POD类型，则逐个构造 */
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy_aux(InputIterator _first, InputIterator _last, ForwardIterator _result, false_type) {
        ForwardIterator cur = _result;
        for(; _first != _last; ++cur, ++_first)
            STL::construct(&*cur, *_first);
        return cur;
    }

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator _first, InputIterator _last, ForwardIterator _result) {
        using isPODType = typename type_traits<value_type_t<InputIterator>>::is_POD_type;
        return uninitialized_copy_aux(_first, _last, _result, isPODType());
    }

    /* 针对char*、wchar_t*存在特化版本 memmove直接移动内存 */
    inline char *uninitialized_copy(const char *_first, const char *_last, char *_result) {
        memmove(_result, _first, _last - _first);
        return _result + (_last - _first);
    }
    inline wchar_t *uninitialized_copy(const wchar_t *_first, const wchar_t *_last, wchar_t *_result) {
        memmove(_result, _first, sizeof(wchar_t) * (_last - _first));
        return _result + (_last - _first);
    }
}


#endif //MYTINYSTL_UNINITIALIZED_H

