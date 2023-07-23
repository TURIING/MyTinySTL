//
// Created by TURIING on 2023/7/19.
//

#ifndef MYTINYSTL_VECTOR_H
#define MYTINYSTL_VECTOR_H

#include "Allocator/Allocator.h"
#include "Allocator/uninitialized.h"
#include "Iterator/ReverseIterator.h"
#include <initializer_list>

namespace STL {
    template<class T, class Alloc = Allocator<T>>
    class Vector {
    public:
        using value_type = T;
        using pointer = value_type *;
        using iterator = value_type *;
        using const_iterator = const value_type *;
        using reverse_iterator = ReverseIterator<iterator>;
        using const_reverse_iterator = ReverseIterator<const_iterator>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
    private:
        using data_allocator = Alloc;

        iterator m_start;
        iterator m_finish;
        iterator m_endOfStorage;

    private:
        void fill_initialize(size_type _n, const value_type &_value);

        iterator allocate_and_fill(size_type _n, const value_type &_value);

        template<class Integer>
        void initialize_aux(Integer _n, Integer _val, true_type);

        template<class InputIterator>
        void initialize_aux(InputIterator _first, InputIterator _last, false_type);

        template<class InputIterator>
        iterator allocate_and_copy(InputIterator _first, InputIterator _last);

        void deallocate() noexcept;

        void destroy_and_deallocate() noexcept;

        void fill_insert(iterator _position, size_type _n, const value_type &_value);

        void insert_aux(iterator, const value_type &);

        template<class InputIterator>
        void range_insert(iterator _pos, InputIterator _first, InputIterator _last, input_iterator_tag);

        template<class ForwardIterator>
        void range_insert(iterator _position, ForwardIterator _first, ForwardIterator _last, forward_iterator_tag);

        template<class Integer>
        void insert_dispatch(iterator _pos, Integer _n, Integer _value, true_type) {
            fill_insert(_pos, static_cast<int>(_n), value_type(_value));
        }

        template<class InputIterator>
        void insert_dispatch(iterator _pos, InputIterator _first, InputIterator _last, false_type) {

        }

    public:
        /***************************************** 构造、析构 *********************************************/
        Vector(): m_start(nullptr), m_finish(nullptr), m_endOfStorage(nullptr) {}

        explicit Vector(size_type _n) { fill_initialize(_n, value_type()); }

        Vector(size_type _n, const value_type &_value) { fill_initialize(_n, _value); }

        template<class InputIterator>
        Vector(InputIterator _first, InputIterator _last) { initialize_aux(_first, _last, STL::is_integral<InputIterator>()); }

        Vector(std::initializer_list<T>);

        Vector(const Vector &);

        Vector(Vector &&) noexcept;

        ~Vector();

        /******************************************** 重载运算符 ********************************************/
        /* 拷贝赋值 */
        Vector &operator=(const Vector &);
        /* 移动赋值 */
        Vector &operator=(Vector &&) noexcept;

        const_reference operator[](const size_type _n) const noexcept { return *(m_start + _n); }
        reference operator[](const size_type _n) noexcept { return *(m_start + _n); }

        bool operator==(const Vector &) const noexcept;

        bool operator!=(const Vector &_rhs) const noexcept { return !(*this == _rhs); }

    public:
        /******************************************* begin、end *************************************************/
        const_iterator begin() const noexcept { return m_start; }
        iterator begin() noexcept { return m_start; }

        const_iterator end() const noexcept { return m_finish; }
        iterator end() noexcept { return m_finish; }

        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(m_finish); }
        reverse_iterator rbegin() noexcept { return reverse_iterator(m_finish); }

        const_reverse_iterator rend() const noexcept { return const_reverse_iterator(m_start); }
        reverse_iterator rend() noexcept { return reverse_iterator(m_start); }

        const_iterator cbegin() const noexcept { return m_start; }
        const_iterator cend() const noexcept { return m_finish; }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_finish); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(m_start); }

        /******************************************* size、capacity *************************************************/
        size_type size() const noexcept { return static_cast<size_type>(m_finish - m_start); }

        size_type capacity() const noexcept { return static_cast<size_type>(m_endOfStorage - m_start); }

        bool empty() const noexcept { return m_start == m_finish; }

        void resize(size_type, const value_type &);
        void resize(size_type _new_size) { resize(_new_size, value_type()); }

        void reserve(size_type _new_capacity);

        void shrink_to_fit() noexcept;

        /* 用于交换两个vector对象的内容 */
        void swap(Vector &) noexcept;

        const_reference front() noexcept { return *begin(); }
        const_reference back() const noexcept { return *(end() - 1); }

        iterator erase(iterator, iterator);
        /* 移除某个元素 */
        iterator erase(iterator _position) { return erase(_position, _position + 1); }

        void push_back(const value_type &);

        void pop_back();

        /* 清空整个vector */
        void clear() { erase(begin(), end()); }

        /********************************************** insert ****************************************************/
        iterator insert(iterator);

        iterator insert(iterator, const value_type &);

        void insert(iterator _pos, size_type _n, const value_type &_value) { fill_insert(_pos, _n, _value); };

        template<class InputIterator>
        void insert(iterator _pos, InputIterator _first, InputIterator _last) { }
    };




}


#endif //MYTINYSTL_VECTOR_H
