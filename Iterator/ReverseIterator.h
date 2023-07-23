//
// Created by TURIING on 2023/7/19.
//

#ifndef MYTINYSTL_REVERSEITERATOR_H
#define MYTINYSTL_REVERSEITERATOR_H

#include <cstddef>
#include "Iterator.h"

namespace STL {


    template<class Iterator>
    class ReverseIterator {
        template<class It>
        friend bool operator==(const ReverseIterator<It> &, const ReverseIterator<It> &);
        template<class It>
        friend bool operator!=(const ReverseIterator<It> &, const ReverseIterator<It> &);
    protected:
        Iterator current;
    public:
        using iterator_type = Iterator;                                                         // 正向迭代器
        using self = ReverseIterator;                                                           // 反向迭代器

        using iterator_category = iterator_category_t<Iterator>;
        using value_type = value_type_t<Iterator>;
        using difference_type = difference_type_t<Iterator>;
        using pointer = pointer_t<Iterator>;
        using reference = reference_type_t<Iterator>;

    public:
        ReverseIterator() {}
        explicit ReverseIterator(iterator_type _value): current(_value) {}
        ReverseIterator(const self &_value): current(_value.current) {}

        /* 用于获取当前反向迭代器所包装的底层正向迭代器 */
        iterator_type base() const { return current; }

        /************************************************* 重载各类运算符 ******************************************/
        bool operator==(const self &_rhs) const { return current == _rhs.current; }

        bool operator!=(const self &_rhs) const { return !((*this) == _rhs); }

        reference operator*() const {
            Iterator temp = current;
            // 对反向迭代器的取值等价于对应的正向迭代器后退一个单位取值
            return *(--temp);
        }

        pointer operator->() const { return  &(operator*()); }

        self &operator++() {
            --current;
            return *this;
        }

        self operator++(int) {
            auto temp = *this;
            --current;
            return temp;
        }

        self &operator--() {
            ++current;
            return *this;
        }

        self operator--(int) {
            auto temp = *this;
            ++current;
            return temp;
        }

        self operator+(difference_type _n) const { return self(current - _n); }

        self &operator+=(difference_type _n) const {
            current -= _n;
            return *this;
        }

        self operator-(difference_type _n) const { return self(current + _n); }

        self &operator-=(difference_type _n) const {
            current += _n;
            return *this;
        }

        reference operator[](difference_type _n) const { return *(*this + _n); }

    };

    template<class Iterator>
    inline bool operator==(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
        return _lhs.operator==(_rhs);
    }

    template<class Iterator>
    inline bool operator!=(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
        return !(_lhs == _rhs);
    }

    template<class Iterator>
    inline bool operator<(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
        return _rhs.base() < _lhs.base();
    }

    template<class Iterator>
    inline bool operator>(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
        return _rhs < _lhs;
    }

    template<class Iterator>
    inline bool operator<=(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
        return !(_rhs < _lhs);
    }

    template<class Iterator>
    inline bool operator>=(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
        return !(_lhs < _rhs);
    }

     template<class Iterator>
     inline typename ReverseIterator<Iterator>::difference_type operator-(const ReverseIterator<Iterator> &_lhs, const ReverseIterator<Iterator> &_rhs) {
         return _rhs.base() - _lhs.base();
     }

    template<class Iterator>
    inline ReverseIterator<Iterator> operator+(typename ReverseIterator<Iterator>::difference_type _n, const ReverseIterator<Iterator> &_x) {
        return ReverseIterator<Iterator>(_x.base() - _n);
    }
}


#endif //MYTINYSTL_REVERSEITERATOR_H
