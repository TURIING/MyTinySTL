//
// Created by TURIING on 2023/7/20.
//

#ifndef MYTINYSTL_TYPE_TRAITS_H
#define MYTINYSTL_TYPE_TRAITS_H

namespace STL {
    /*********************************************** true_type/false_type ******************************************/
    template<class T, T val>
    struct integral_constant {
        static constexpr T value = val;
        using value_type = T;
        using type = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };
     template<class T, T val>
     constexpr T integral_constant<T, val>::value;

    template<bool val>
    using bool_constant = integral_constant<bool, val>;

    struct true_type: bool_constant<true> {};
    struct false_type: bool_constant<false> {};

    /************************************************* type_traits **************************************************/
    template<class T>
    struct type_traits {
        using has_trivial_default_constructor = false_type;
        using has_trivial_copy_constructor = false_type;
        using has_trivial_assignment_operator = false_type;
        using has_trivial_destructor = false_type;
        using is_POD_type = false_type;
    };

    template<>
    struct type_traits<bool> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<char> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<unsigned char> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<signed char> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<wchar_t> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<short> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<unsigned short > {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<int> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<unsigned int> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<long> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<unsigned long> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<long long> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<unsigned long long> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<float> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<double> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<long double> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<class T>
    struct type_traits<T *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<class T>
    struct type_traits<const T *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<char *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<unsigned char *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<signed char *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<const char *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<const unsigned char *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    template<>
    struct type_traits<const signed char *> {
        using has_trivial_default_constructor = true_type;
        using has_trivial_copy_constructor = true_type;
        using has_trivial_assignment_operator = true_type;
        using has_trivial_destructor = true_type;
        using is_POD_type = true_type;
    };

    /****************************************** is_integral **********************************************************/
    template<class T>
    struct is_integral: false_type {};                                  // 自定义类型默认为false

    /*以下为特定类型的特化 */
    template<>
    struct is_integral<bool>: true_type{};

    template<>
    struct is_integral<char>: true_type{};

    template<>
    struct is_integral<signed char>: true_type{};

    template<>
    struct is_integral<unsigned char>: true_type{};

    template<>
    struct is_integral<wchar_t>: true_type{};

    template<>
    struct is_integral<short>: true_type{};

    template<>
    struct is_integral<unsigned short>: true_type{};

    template<>
    struct is_integral<int>: true_type{};

    template<>
    struct is_integral<unsigned int>: true_type{};

    template<>
    struct is_integral<long>: true_type{};

    template<>
    struct is_integral<unsigned long>: true_type{};

    template<>
    struct is_integral<long long>: true_type{};

    template<>
    struct is_integral<unsigned long long>: true_type{};
}


#endif //MYTINYSTL_TYPE_TRAITS_H
