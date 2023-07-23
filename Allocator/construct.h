//
// Created by TURIING on 2023/7/20.
//

#ifndef MYTINYSTL_CONSTRUCT_H
#define MYTINYSTL_CONSTRUCT_H

namespace STL {
    template<class T1, class T2>
    inline void construct(T1 *_ptr, T2 _value) {
        new (_ptr) T1(_value);
    }

    /********************************************** destroy *******************************************************/
    template<class T>
    inline void destroy(T *_ptr) {
        _ptr->~T();
    }

    /* 如果元素的类型不是POD，则存在默认析构函数，逐个析构 */
    template<class ForwardIterator>
    inline void destroy_aux(ForwardIterator _beg, ForwardIterator _end, false_type) {
        for(; _beg != _end; ++_beg) destroy(&*_beg);
    }
    /* 如果元素的类型是POD，则不需要析构 */
    template<class ForwardIterator>
    inline void destroy_aux(ForwardIterator, ForwardIterator, true_type) {}

    template<class ForwardIterator>
    inline void destroy(ForwardIterator _beg, ForwardIterator _end) {
        using is_POD_type = typename type_traits<ForwardIterator>::is_POD_type;
        destroy_aux(_beg, _end, is_POD_type());
    }
    /* 针对char*的特化 */
    inline void destroy(char *, char *) {}
    /* 针对wchar_t*的特化 */
    inline void destroy(wchar_t *, wchar_t *) {}
}

#endif //MYTINYSTL_CONSTRUCT_H
