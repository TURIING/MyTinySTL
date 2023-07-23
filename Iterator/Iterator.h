//
// Created by TURIING on 2023/7/20.
//

#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H
#include <cstddef>

namespace STL {
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class T>
    struct iterator_traits<T *> {
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = T*;
        using reference = T&;
    };
    template<class T>
    struct iterator_traits<const T*>{
        using iterator_category = random_access_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
    };

    template<class Iterator>
    using iterator_category_t = typename iterator_traits<Iterator>::iterator_category;

    template<class Iterator>
    using value_type_t = typename iterator_traits<Iterator>::value_type;

    template<class Iterator>
    using difference_type_t = typename iterator_traits<Iterator>::difference_type ;

    template<class Iterator>
    using pointer_t = typename iterator_traits<Iterator>::pointer;

    template<class Iterator>
    using reference_type_t = typename iterator_traits<Iterator>::reference;

    /************************************************ distance ****************************************************/
    template<class InputIterator>
    inline difference_type_t<InputIterator> distance_aux(InputIterator _first, InputIterator _last, input_iterator_tag) {
        difference_type_t<InputIterator> n = 0;
        while (_first != _last) ++_first, ++n;
        return n;
    }

    template<class InputIterator>
    inline difference_type_t<InputIterator> distance_aux(InputIterator _first, InputIterator _last, random_access_iterator_tag) {
        return _last - _first;
    }

    template<class InputIterator>
    inline difference_type_t<InputIterator> distance(InputIterator _first, InputIterator _last) {
        return distance_aux(_first, _last, iterator_category_t<InputIterator>());
    }
}

#endif //MYTINYSTL_ITERATOR_H
