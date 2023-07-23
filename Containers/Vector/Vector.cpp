//
// Created by TURIING on 2023/7/19.
//

#include "Vector.h"

namespace STL {
    template<class T, class Alloc>
    void Vector<T, Alloc>::fill_initialize(Vector::size_type _n, const value_type &_value) {
        m_start = allocate_and_fill(_n, _value);
        m_finish = m_start + _n;
        m_endOfStorage = m_finish;
    }

    template<class T, class Alloc>
    Vector<T, Alloc>::iterator Vector<T, Alloc>::allocate_and_fill(Vector::size_type _n, const value_type &_value) {
        iterator result = data_allocator::allocate(_n);
        STL::uninitialized_fill_n(result, _n, _value);
        return result;
    }

    template<class T, class Alloc>
    template<class Integer>
    void Vector<T, Alloc>::initialize_aux(Integer _n, Integer _val, true_type) {
        fill_initialize(static_cast<size_type>(_n), static_cast<value_type>(_val));
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void Vector<T, Alloc>::initialize_aux(InputIterator _first, InputIterator _last, false_type) {
        m_start = allocate_and_copy(_first, _last);
        m_finish = m_endOfStorage = m_start + STL::distance(_first, _last);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    Vector<T, Alloc>::iterator Vector<T, Alloc>::allocate_and_copy(InputIterator _first, InputIterator _last) {
        size_type n = STL::distance(_first, _last);
        iterator result = data_allocator::allocate(n);
        STL::uninitialized_copy(_first, _last, result);
        return result;
    }

    template<class T, class Alloc>
    Vector<T, Alloc>::Vector(std::initializer_list<T> _list) {
        m_start = allocate_and_copy(_list.begin(), _list.end());
        m_finish = m_endOfStorage = m_start + _list.size();
    }

    template<class T, class Alloc>
    Vector<T, Alloc>::Vector(const Vector & _rhs) {
        m_start = allocate_and_copy(_rhs.begin(), _rhs.end());
        m_finish = m_endOfStorage = m_start + _rhs.size();
    }

    template<class T, class Alloc>
    inline Vector<T, Alloc>::Vector(Vector &&_rhs) noexcept {
        m_start = _rhs.m_start;
        m_finish  = _rhs.m_finish;
        m_endOfStorage = _rhs.m_endOfStorage;
        _rhs.m_start = _rhs.m_finish = _rhs.m_endOfStorage = nullptr;
    }

    template<class T, class Alloc>
    Vector<T, Alloc>::~Vector() {
        STL::destroy(m_start, m_finish);
        deallocate();
    }

    template<class T, class Alloc>
    void Vector<T, Alloc>::deallocate() noexcept {
        if(m_start)
            data_allocator::deallocate(m_start, m_endOfStorage - m_start);
    }

    template<class T, class Alloc>
    inline Vector<T, Alloc> &Vector<T, Alloc>::operator=(const Vector &_rhs) {
        Vector temp(_rhs);
        swap(temp);
        return *this;
    }

    template<class T, class Alloc>
    Vector<T, Alloc> &Vector<T, Alloc>::operator=(Vector &&_rhs) noexcept {
        if(this != &_rhs) {
            destroy_and_deallocate();
            m_start = _rhs.m_start;
            m_finish = _rhs.m_finish;
            m_endOfStorage = _rhs.m_endOfStorage;
            _rhs.m_start = _rhs.m_finish = _rhs.m_endOfStorage = nullptr;
        }
        return *this;
    }

    template<class T, class Alloc>
    inline void Vector<T, Alloc>::swap(Vector &_rhs) noexcept {
        STL::swap(m_start, _rhs.m_start);
        STL::swap(m_finish, _rhs.m_finish);
        STL::swap(m_endOfStorage, _rhs.m_endOfStorage);
    }

    template<class T, class Alloc>
    void Vector<T, Alloc>::destroy_and_deallocate() noexcept {
        STL::destroy(m_start, m_finish);
        deallocate();
    }

    template<class T, class Alloc>
    inline void Vector<T, Alloc>::resize(size_type _new_size, const value_type &_value) {
        if(_new_size < size())
            erase(begin() + _new_size, end());
        else
            fill_insert(end(), _new_size - size(), _value);
    }

    /*
     * 移除指定范围的元素
     */
    template<class T, class Alloc>
    Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(iterator _first, iterator _last) {
        /* 将欲擦除的元素覆盖掉 */
        iterator i = STL::copy(_last, m_finish, _first);
        /* 析构掉后面的元素 */
        STL::destroy(i, m_finish);

        m_finish -= (_last - _first);
        return _first;
    }

    template<class T, class Alloc>
    void Vector<T, Alloc>::fill_insert(Vector::iterator _position, Vector::size_type _n, const value_type &_value) {
        assert(_n > 0);

        /* 剩余空间大于欲插入的大小 */
        if(static_cast<size_type>(m_endOfStorage - m_finish) >= _n) {
            value_type value_copy = _value;
            const size_type  elems_after = m_finish - _position;                                                    // position之后的元素个数
            iterator old_finish = m_finish;
            if(elems_after > _n) {
                STL::uninitialized_copy(m_finish - _n, m_finish, m_finish);
                m_finish += _n;
                STL::copy_backward(_position, old_finish - _n, value_copy);
                STL::fill(_position, _position + n, value_copy);
            } else {
                STL::uninitialized_fill_n(m_finish, _n - elems_after, value_copy);
                m_finish += _n - elems_after;
                STL::uninitialized_copy(_position, old_finish, m_finish);
                m_finish += elems_after;
                STL::fill(_position, old_finish, value_copy);
            }
        }
        /* 剩余空间小于欲插入大小 */
        else {
            const size_type old_size = size();
            const size_type new_size = old_size + STL::max(old_size, _n);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;

            try {
                new_finish = STL::uninitialized_copy(m_start, _position, new_start);
                new_finish = STL::uninitialized_fill_n(new_finish, _n, _value);
                new_finish = STL::uninitialized_copy(_position, m_finish, new_finish);
            } catch (std::exception &) {
                STL::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }

            destroy_and_deallocate();
            m_start = new_start;
            m_finish = new_finish;
            m_endOfStorage = new_start + new_size;
        }
    }

    /*
     * 用于预分配内存空间以容纳指定数量的元素
     */
    template<class T, class Alloc>
    inline void Vector<T, Alloc>::reserve(Vector::size_type _new_capacity) {
        if(_new_capacity <= capacity()) return;

        T *new_start = data_allocator::allocate(_new_capacity);
        T *new_finish = STL::uninitialized_copy(m_start, m_finish, new_start);
        destroy_and_deallocate();
        m_start = new_start;
        m_finish = new_finish;
        m_endOfStorage = m_start + _new_capacity;
    }

    /*
     * 用于要求 vector 缩小其容量以适应其当前大小
     */
    template<class T, class Alloc>
    void Vector<T, Alloc>::shrink_to_fit() noexcept {
        Vector temp(*this);
        swap(temp);
    }

    template<class T, class Alloc>
    bool Vector<T, Alloc>::operator==(const Vector &_rhs) const noexcept {
        if(size() != _rhs.size())
            return false;
        else {
            iterator ptr1 = m_start;
            iterator ptr2 = _rhs.m_start;
            for(; ptr1 != m_finish && ptr2 != _rhs.m_finish; ++ptr1, ++ptr2)
                if(*ptr1 != *ptr2) return false;
            return true;
        }
    }

    /*
     * 在尾端插入数据
     */
    template<class T, class Alloc>
    void Vector<T, Alloc>::push_back(const value_type &_value) {
        if(m_finish != m_endOfStorage){
            STL::construct(m_finish, _value);
            ++m_finish;
        } else
            insert_aux(end(), _value);
    }

    /*
     * 弹出尾端的数据
     */
    template<class T, class Alloc>
    void Vector<T, Alloc>::pop_back() {
        --m_finish;
        STL::destroy(m_finish);
    }

    /*
     * 在position处插入数据
     */
    template<class T, class Alloc>
    void Vector<T, Alloc>::insert_aux(iterator _position, const value_type &_value) {
        /* 有足够空间插入新数据, 则无需扩容 */
        if(m_finish != m_endOfStorage) {
            STL::construct(m_finish, *(m_finish - 1));
            ++m_finish;
            value_type value_copy = _value;
            STL::copy_backward(_position, m_finish - 2, m_finish - 1);
            *_position = value_copy;
        }
        /* 没有足够空间插入，需要扩容 */
        else {
            const size_type old_size = size();
            const size_type new_size = old_size ? (2 * old_size) : 1;
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try {
                new_finish = STL::uninitialized_copy(m_start, _position, new_start);
                STL::construct(new_finish, _value);
                ++new_finish;
                new_finish = STL::uninitialized_copy(_position, m_finish, new_finish);
            } catch (std::exception &) {
                STL::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            destroy_and_deallocate();
            m_start = new_start;
            m_finish = new_finish;
            m_endOfStorage = new_start + new_size;
        }
    }

    /*
     * 在position处插入默认值
     */
    template<class T, class Alloc>
    Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(iterator _position) {
        return insert(_position, value_type());
    }

    /*
     * 在position处插入给定的数据
     */
    template<class T, class Alloc>
    Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(iterator _position, const value_type &_value) {
        size_type n = _position - begin();
        if(m_finish != m_endOfStorage && _position == end()) {
            STL::construct(m_finish, _value);
            ++m_finish;
        }else
            insert_aux(_position, _value);
        return begin() + n;
    }

    /*
     * 在pos处插入[first, last]内的数据
     */
    template<class T, class Alloc>
    template<class InputIterator>
    void Vector<T, Alloc>::range_insert(Vector::iterator _pos, InputIterator _first, InputIterator _last, input_iterator_tag) {
        for(; _first != _last; ++_first) {
            _pos = insert(_pos, *_first);
            ++_pos;
        }
    }

    template<class T, class Alloc>
    template<class ForwardIterator>
    void Vector<T, Alloc>::range_insert(Vector::iterator _position, ForwardIterator _first, ForwardIterator _last, forward_iterator_tag) {
        if(_first != _last) {
            size_type n = STL::distance(_first, _last);
            if(static_cast<size_type>(m_endOfStorage - m_finish) >= n) {
                const size_type elems_after = m_finish - _position;
                iterator old_finish = m_finish;
                if(elems_after > n) {
                    STL::uninitialized_copy(m_finish - n, m_finish, m_finish);
                    m_finish += n;
                    STL::copy_backward(_position, old_finish - n, old_finish);
                    STL::copy(_position, _position + n, _position);
                } else {
                    ForwardIterator mid = _first;

                }
            }
        }
    }
}