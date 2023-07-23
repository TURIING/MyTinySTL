//
// Created by TURIING on 2023/7/20.
//

#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H

namespace STL {
    /***************************************** fill ******************************************************/
    /*
     * 用value填充进[first, last]范围内的元素
     */
    template<class ForwardIterator, class T>
    void fill(ForwardIterator _first, ForwardIterator _last, const T &_value) {
        for(; _first != _last; ++_first)
            *_first = _value;
    }

    /*
     * 用value填充first后面的n个元素
     */
    template<class OutputIterator, class Size, class T>
    OutputIterator fill_n(OutputIterator _first, Size _n, const T &_value) {
        for(; _n > 0; --_n, ++_first)
            *_first = _value;
        return _first;
    }

    /*************************************************** copy ***************************************************/
    template<class InputIterator, class OutputIterator, class Distance>
    inline OutputIterator copy_d(InputIterator _first, InputIterator _last, OutputIterator _result, Distance) {
        for(Distance n = _last - _first; n > 0; --n, ++_first, ++_result)
            *_result = *_first;
        return _result;
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator copy_aux(InputIterator _first, InputIterator _last, OutputIterator _result, input_iterator_tag) {
        for(; _first != _last; ++_first, ++_result)
            *_result = *_first;
        return _result;
    }
    template<class InputIterator, class OutputIterator>
    inline OutputIterator copy_aux(InputIterator _first, InputIterator _last, OutputIterator _result, random_access_iterator_tag) {
        return copy_d(_first, _last, _result, difference_type_t<InputIterator>());
    }

    /* 具备trivial copy assignment operator，可执行memmove */
    template<class T>
    inline T *copy_t(const T *_first, const T *_last, T *_result, true_type) {
        memmove(_result, _first, sizeof(T) * (_last - _first));
        return _result + (_last - _first);
    }
    /* 原始指针是一种random_access_iterator */
    template<class T>
    inline T *copy_t(const T *_first, const T *_last, T *_result, false_type) {
        return copy_d(_first, _last, _result, ptrdiff_t());
    }

    // 仿函数对象
    template<class InputIterator, class OutputIterator>
    struct copy_dispatch {
        OutputIterator operator()(InputIterator _first, InputIterator _last, OutputIterator _result) {
            return copy_aux(_first, _last, _result, iterator_category_t<InputIterator>());
        }
    };
    template<class T>
    struct copy_dispatch<T *, T *> {
        T *operator()(T *_first, T *_last, T *_result) {
            using t = typename type_traits<T>::has_trivial_assignment_operator;
            return copy_t(_first, _last, _result, t());
        }
    };
    template<class T>
    struct copy_dispatch<const T*, T*>{
        T *operator()(const T *_first, const T *_last, T *_result) {
            using t = typename type_traits<T>::has_trivial_assignment_operator;
            return copy_t(_first, _last, _result, t());
        }
    };
    /*
     * 根据类型复制元素，返回复制的最后元素的后一个元素地址
     */
    template<class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator _first, InputIterator _last, OutputIterator _result) {
        return copy_dispatch<InputIterator, OutputIterator>()(_first, _last, _result);
    }
    // 针对指针的偏特化
    inline char *copy(const char *_first, const char *_last, char *_result) {
        memmove(_result, _first, _last - _first);
        return _result + (_last - _first);
    }
    inline wchar_t *copy(const wchar_t  *_first, const wchar_t  *_last, wchar_t  *_result) {
        memmove(_result, _first, sizeof(wchar_t ) * (_last - _first));
        return _result + (_last - _first);
    }
    /************************************************ copy_backward ***********************************************/
    template<class BidirectionalIter1, class BidirectionalIter2, class Distance>
    inline BidirectionalIter2 copy_backward_aux(BidirectionalIter1 _first, BidirectionalIter1 _last, BidirectionalIter2 _result, bidirectional_iterator_tag, Distance) {
        while (_first != _last) *(--_result) = *(--_last);
        return _result;
    }
    template<class RandomAccessIter, class BidirectionalIter2, class Distance>
    inline BidirectionalIter2 copy_backward_aux(RandomAccessIter _first, RandomAccessIter _last, BidirectionalIter2 _result, random_access_iterator_tag, Distance) {
        for(Distance n = _last - _first; n > 0; --n) *(--_result) = *(--_last);
        return _result;
    }

    template<class BidirectionalIter1, class BidirectionalIter2, class BoolType>
    struct copy_backward_dispatch {
        BidirectionalIter2 operator() (BidirectionalIter1 _first, BidirectionalIter1 _last, BidirectionalIter2 _result) {
            return copy_backward_aux(_first, _last, _result, iterator_category_t<BidirectionalIter1>(), difference_type_t<BidirectionalIter1>());
        }
    };
    template<class T>
    struct copy_backward_dispatch<T *, T *, true_type> {
        T *operator()(const T *_first, const T *_last, T *_result) {
            const ptrdiff_t n = _last - _first;
            memmove(_result - n, _first, sizeof(T) * n);
            return _result - n;
        }
    };
    template<class T>
    struct copy_backward_dispatch<const T *, T *, true_type>{
        T *operator()(const T *_first, const T *_last, T *_result) {
            return copy_backward_dispatch<T *, T *, true_type>()(_first, _last, _result);
        }
    };

    /*
     * 将[first，last]范围内的数据往后移动，从result开始往前填充
     */
    template<class BI1, class BI2>
    inline BI2 copy_backward(BI1 _first, BI1 _last, BI2 _result) {
        using trivial = typename type_traits<value_type_t<BI2>>::has_trivial_assignment_operator;
        return copy_backward_dispatch<BI1, BI2, trivial>()(_first, _last, _result);
    }

    template<class T>
    inline void swap(T &_a, T &_b) {
        T temp = _a;
        _a = _b;
        _b = temp;
    }

    /*********************************************** max、min ******************************************************/
    template<class T>
    inline const T &max(const T &_a, const T &_b) {
        return _a < _b ? _b : _a;
    }

    template<class T, class Compare>
    inline const T &max(const T &_a, const T &_b, Compare comp) {
        return comp(_a, _b) ? _b : _a;
    }
}

#endif //MYTINYSTL_ALGOBASE_H
