//
// Created by Brunshweeck on 11/09/2023 17:57.
// Copyright (c) 2023
//

#ifndef CORE23_TEMPLATES_H
#define CORE23_TEMPLATES_H

#include <core/Private/Generics.h>

namespace core {

    namespace native {

        namespace CORE_DEPRECATED Templates {


            CORE_WARNING_PUSH
            CORE_WARNING_DISABLE_DEPRECATED

            template<gbool X>
            interface CONSTANT {
                static CORE_FAST gbool Value = X;
            };

            template<class Alias>
            interface ALWAYS {
                CORE_ALIAS(Type, Alias);
            };

            template<class ...>
            CORE_ALIAS(IGNORED, void);

            CORE_ALIAS(ALWAYS_TRUE, CONSTANT<1>);
            CORE_ALIAS(ALWAYS_FALSE, CONSTANT<0>);

            template<gint Mask, class...>
            interface CORE_DEPRECATED TEST : ALWAYS_FALSE {
            };

            /**
             * Custom type
             */
            template<gint Mask, class Alias, gint Size = -1>
            interface CORE_DEPRECATED MODIFY : ALWAYS<Alias> {
            };



            /////////////////////////////////[Similarity]/////////////////////////////////////

#if __has_builtin(__is_same) || defined(__clang__)
            template<class T, class U>
            interface TEST<1, T, U>: CONSTANT<__is_same(T, U)> {};
#else
            template<class T>
            interface TEST<1, T, T> : ALWAYS_TRUE {
            };
#endif

            /////////////////////////////////[Constant]/////////////////////////////////////

            template<class T>
            interface TEST<2, const T> : ALWAYS_TRUE {
            };

            template<>
            interface MODIFY<2, void> : ALWAYS<void> {
            };

            template<class T>
            interface MODIFY<2, T> : ALWAYS<const T> {
            };

            template<class T>
            interface MODIFY<-2, const T> : ALWAYS<T> {
            };



            /////////////////////////////////[Volatile]/////////////////////////////////////

            template<class T>
            interface TEST<3, volatile T> : ALWAYS_TRUE {
            };

            template<>
            interface MODIFY<3, void> : ALWAYS<void> {
            };

            template<class T>
            interface MODIFY<3, T> : ALWAYS<volatile T> {
            };

            template<class T>
            interface MODIFY<-3, volatile T> : ALWAYS<T> {
            };



            /////////////////////////////////[Reference]/////////////////////////////////////

            template<class T>
            interface TEST<4, T &> : ALWAYS_TRUE {
            };

            template<class T>
            interface TEST<4, T &&> : ALWAYS_TRUE {
            };

            template<class T>
            interface TEST<44, T &&> : ALWAYS_TRUE {
            };

            template<class T>
            interface TEST<444, T &> : ALWAYS_TRUE {
            };

            template<>
            interface MODIFY<4, void> : ALWAYS<void> {
            };

            template<>
            interface MODIFY<44, void> : ALWAYS<void> {
            };

            template<class T>
            interface MODIFY<4, T> : ALWAYS<T &> {
            };

            template<class T>
            interface MODIFY<44, T> : ALWAYS<T &&> {
            };

            template<class T>
            interface MODIFY<-4, T &> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-4, T &&> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-44, T &&> : ALWAYS<T> {
            };
            /////////////////////////////////[Pointer]/////////////////////////////////////

            template<class T>
            interface TEST<5, T *> : ALWAYS_TRUE {
            };

            template<class T>
            interface MODIFY<5, T> : ALWAYS<T *> {
            };

            template<class T>
            interface MODIFY<5, T &> : ALWAYS<T *> {
            };

            template<class T>
            interface MODIFY<5, T &&> : ALWAYS<T *> {
            };

            template<class T>
            interface MODIFY<-5, T *> : ALWAYS<T> {
            };

            /////////////////////////////////[ConstVol]/////////////////////////////////////

            template<>
            interface MODIFY<6, void> : ALWAYS<void> {
            };

            template<class T>
            interface MODIFY<6, T> : ALWAYS<const volatile T> {
            };

            template<class T>
            interface MODIFY<-6, const T> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-6, volatile T> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-6, const volatile T> : ALWAYS<T> {
            };

            /////////////////////////////////[Array]/////////////////////////////////////

            template<class T>
            interface TEST<9, T[]> : ALWAYS_TRUE {
            };

            template<class T, gint Size>
            interface TEST<9, T[Size]> : ALWAYS_TRUE {
            };

            template<class T, gint Size>
            interface MODIFY<9, T, Size> : ALWAYS<T[Size]> {
            };

            template<class T>
            interface MODIFY<9, T, -1> : ALWAYS<T[]> {
            };

            template<class T>
            interface MODIFY<-9, T[]> : ALWAYS<T> {
            };

            template<class T, gint Size>
            interface MODIFY<-9, T[Size]> : ALWAYS<T> {
            };

            /////////////////////////////////[Template]/////////////////////////////////////

            template<template<class ...TParams> class T, class ...TParams>
            interface TEST<11, T<TParams...>> : ALWAYS_TRUE {
            };

            /////////////////////////////////[function]/////////////////////////////////////

            template<class T>
            interface TEST<12, T> : TEST<1, T, const T> {
            };

            template<class R>
            interface TEST<12, R(...)> : ALWAYS_TRUE {
            };

            template<class R, class...Params>
            interface TEST<12, R(Params...)> : ALWAYS_TRUE {
            };

            /////////////////////////////////[Class Member]/////////////////////////////////////
#ifdef __clang__
            template <class T>
            interface TEST<13, T>: CONSTANT<__is_member_pointer(T)> {};
#else
            template<class T, class U>
            interface TEST<13, T U::*> : ALWAYS_TRUE {
            };
#endif
            /////////////////////////////////[function Member]/////////////////////////////////////
#ifdef __clang__
            template <class T>
            interface TEST<14, T>: CONSTANT<__is_member_function_pointer(T)> {};
#else
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...)> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) volatile> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const volatile> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) &> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const &> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) volatile &> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const volatile &> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) &&> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const &&> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) volatile &&> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const volatile &&> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) volatile CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const volatile CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) &CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const &CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) volatile &CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const volatile &CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) &&CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const &&CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) volatile &&CORE_NOTHROW> : ALWAYS_TRUE {
            };
            template<class T, class U, class ...Params>
            interface TEST<14, T(U::*)(Params...) const volatile &&CORE_NOTHROW> : ALWAYS_TRUE {
            };
#endif
            /////////////////////////////////[Abstract]/////////////////////////////////////

            template<class T>
            interface TEST<15, T> : CONSTANT<__is_abstract(T)> {
            };

            /////////////////////////////////[Enum]/////////////////////////////////////

            template<class T>
            interface TEST<16, T> : CONSTANT<__is_enum(T)> {
            };

            /////////////////////////////////[Class]/////////////////////////////////////

            template<class T>
            interface TEST<17, T> : CONSTANT<__is_class(T)> {
            };

            /////////////////////////////////[Constructible]/////////////////////////////////////

            template<class T, class...Params>
            interface TEST<18, T, Params...> : CONSTANT<__is_constructible(T, Params...)> {
            };

            /////////////////////////////////[Assignable]/////////////////////////////////////

            template<class From, class To>
            interface TEST<19, From, To> : CONSTANT<__is_assignable(To, From)> {
            };

            /////////////////////////////////[Super]/////////////////////////////////////

            template<class T, class U>
            interface TEST<20, T, U> : CONSTANT<__is_base_of(T, U)> {
            };

            /////////////////////////////////[Conditional]/////////////////////////////////////

            template<class T, gbool>
            interface CONDITIONAL_STRICT : ALWAYS_FALSE {
            };

            template<class T>
            interface CONDITIONAL_STRICT<T, true> : ALWAYS<T> {
            };

            template<class T, gbool, class Fallback>
            interface CONDITIONAL : ALWAYS<Fallback> {
            };

            template<class T, class U>
            interface CONDITIONAL<T, true, U> : ALWAYS<T> {
            };

            // END
        }

    }
}

namespace {


#ifdef CORE_COMPILER_MSVC
#ifdef _DComplex
    CORE_ALIAS(GENERIC_COMPLEX, _DComplex);
#else
    CORE_ALIAS(GENERIC_COMPLEX, struct _C_double_complex);
#endif
#else
    CORE_ALIAS(GENERIC_CPLEX, _Complex double);
#endif

}

namespace core {

    namespace native {

        namespace Templates {

            /////////////////////////////////[Declaration]/////////////////////////////////////
            namespace {

                template<class T, class U = T &&>
                static U UNSAFE_DECLARATION(gint);

                template<class T>
                static T UNSAFE_DECLARATION(glong);

                template<class T>
                static decltype(UNSAFE_DECLARATION<T>(0)) FALSE_DECLARATION();

            }


            /////////////////////////////////[Convertible]/////////////////////////////////////
#ifdef CORE_COMPILER_MSVC
            template<class From, class To>
            interface TEST<21, From, To> : CONSTANT<__is_convertible_to(From, To)> {
            };
#else
            namespace {
                template<class T>
                void UNSAFE_IMPLICIT_CONVERTER(T);

                template<class From, class To, class = decltype(UNSAFE_IMPLICIT_CONVERTER<To>(
                        UNSAFE_DECLARATION<From>()))>
                static ALWAYS_TRUE UNSAFE_IMPLICIT_CONVERTER_TEST(gint);

                template<class, class>
                static ALWAYS_FALSE UNSAFE_IMPLICIT_CONVERTER_TEST(glong);

            }
            template<class From, class To>
            interface TEST<21, From, To> : decltype(UNSAFE_IMPLICIT_CONVERTER_TEST<From, To>(0)) {
            };
#endif


            /////////////////////////////////[Complete]/////////////////////////////////////

            template<class T>
            interface TEST<10, T &> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, T &&> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, const T> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, volatile T> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, const volatile T> : TEST<10, T> {
            };

#ifdef __clang__
            template<class T>
            interface TEST<10, T> : CONSTANT<__is_complete_type(T)> {
            };
#else
            namespace {

                template<class T, gint Size = sizeof(T)>
                static ALWAYS_TRUE SIZE_COMPUTE_TESTER(gint);

                template<class T>
                static ALWAYS_FALSE SIZE_COMPUTE_TESTER(glong);

            }

            template<class T>
            interface TEST<10, T> : decltype(SIZE_COMPUTE_TESTER<T>(0)) {
            };
#endif
            template<class T>
            interface TEST<10, T[]> : ALWAYS_FALSE {
            };

            template<class T>
            interface TEST<10, T *> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<10, void> : ALWAYS_FALSE {
            };

            template<>
            interface TEST<10, Void> : ALWAYS_FALSE {
            };

            /////////////////////////////////[Integer]/////////////////////////////////////
            template<class T>
            interface TEST<22, T &> : TEST<22, T> {
            };
            template<class T>
            interface TEST<22, T &&> : TEST<22, T> {
            };
            template<class T>
            interface TEST<22, const T> : TEST<22, T> {
            };
            template<class T>
            interface TEST<22, volatile T> : TEST<22, T> {
            };
            template<class T>
            interface TEST<22, const volatile T> : TEST<22, T> {
            };
            template<>
            interface TEST<22, gbyte> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, gshort> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, unsigned short> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, gint> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, unsigned int> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, long> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, unsigned long> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, long long> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, unsigned long long> : ALWAYS_TRUE {
            };
#if defined(__INT128__) || defined(__SIZEOF_INT128__)
            template<>
            interface TEST<22, __int128> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<22, unsigned __int128> : ALWAYS_TRUE {
            };
#endif

            /////////////////////////////////[Floating]/////////////////////////////////////
            template<class T>
            interface TEST<23, T &> : TEST<23, T> {
            };
            template<class T>
            interface TEST<23, T &&> : TEST<23, T> {
            };
            template<class T>
            interface TEST<23, const T> : TEST<23, T> {
            };
            template<class T>
            interface TEST<23, volatile T> : TEST<23, T> {
            };
            template<class T>
            interface TEST<23, const volatile T> : TEST<23, T> {
            };
            template<>
            interface TEST<23, gfloat> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<23, gdouble> : ALWAYS_TRUE {
            };
#if defined(__FLOAT128__) || defined(__SIZEOF_FLOAT128__)
            template<>
            interface TEST<23, __float128> : ALWAYS_TRUE {
            };
#endif
            /////////////////////////////////[Character]/////////////////////////////////////
            template<class T>
            interface TEST<24, T &> : TEST<24, T> {
            };
            template<class T>
            interface TEST<24, T &&> : TEST<24, T> {
            };
            template<class T>
            interface TEST<24, const T> : TEST<24, T> {
            };
            template<class T>
            interface TEST<24, volatile T> : TEST<24, T> {
            };
            template<class T>
            interface TEST<24, const volatile T> : TEST<24, T> {
            };
            template<>
            interface TEST<24, char> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<24, wchar_t> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<24, char16_t> : ALWAYS_TRUE {
            };
            template<>
            interface TEST<24, char32_t> : ALWAYS_TRUE {
            };
#ifdef __cpp_lib_char8_t
            template<>
            interface TEST<24, char8_t> : ALWAYS_TRUE {
            };
#endif

            /////////////////////////////////[Boolean]/////////////////////////////////////
            template<class T>
            interface TEST<25, T &> : TEST<25, T> {
            };
            template<class T>
            interface TEST<25, T &&> : TEST<25, T> {
            };
            template<class T>
            interface TEST<25, const T> : TEST<25, T> {
            };
            template<class T>
            interface TEST<25, volatile T> : TEST<25, T> {
            };
            template<class T>
            interface TEST<25, const volatile T> : TEST<25, T> {
            };
            template<>
            interface TEST<25, GENERIC_BOOL> : ALWAYS_TRUE {
            };

            /////////////////////////////////[Callable]/////////////////////////////////////

            template<class T, class ...Aliases>
            interface TEST<26, T(*)(Aliases...)> : ALWAYS_TRUE, ALWAYS<T> {
            };
            template<class T, class U, class ...Aliases>
            interface TEST<26, T(U::*)(Aliases...)> : ALWAYS_TRUE, ALWAYS<T> {
            };

            namespace {
                template<class T, class ...U>
                static ALWAYS<decltype(FALSE_DECLARATION<T>()(FALSE_DECLARATION<U>()...))>
                UNSAFE_FUNCTION_TESTER(gint);

                template<class ...>
                static ALWAYS_FALSE UNSAFE_FUNCTION_TESTER(glong);

                template<class T, class U, class ...V>
                static ALWAYS<decltype((FALSE_DECLARATION<U>().*FALSE_DECLARATION<T>())(
                        FALSE_DECLARATION<V>()...))> UNSAFE_METHOD_TESTER(gint);

                template<class ...>
                static ALWAYS_FALSE UNSAFE_METHOD_TESTER(glong);

                template<class T, class U, class ...V>
                static ALWAYS<decltype(((*FALSE_DECLARATION<U>()).*FALSE_DECLARATION<T>())(
                        FALSE_DECLARATION<V>()...))> UNSAFE_MEMBER_TESTER(gint);

                template<class ...>
                static ALWAYS_FALSE UNSAFE_MEMBER_TESTER(glong);
            }

            template<class T, class...U>
            interface TEST<26, T, U...> {
                CORE_ALIAS(Type, decltype(UNSAFE_FUNCTION_TESTER<T, U...>(0)));
                static CORE_FAST gbool Value = !TEST<1, Type, ALWAYS_FALSE>::Value;
            };

            template<class T, class U, class V, class ...W>
            interface TEST<26, T U::*, V, W...> {
                CORE_ALIAS(V2, typename MODIFY<-4,, V > ::Type);
                CORE_ALIAS(Type1, decltype(UNSAFE_METHOD_TESTER<T U::*, V2, W...>(0)));
                CORE_ALIAS(Type2, decltype(UNSAFE_MEMBER_TESTER<T U::*, V2, W...>(0)));
                static CORE_FAST gbool Value1 = !TEST<1, Type1, ALWAYS_FALSE>::Value;
                static CORE_FAST gbool Value2 = !TEST<1, Type2, ALWAYS_FALSE>::Value;
                static CORE_FAST gbool Value = Value1 || Value2;
                CORE_ALIAS(Type, typename CONDITIONAL<Type1,, Value1, Type2 > ::Type);
            };


            /////////////////////////////////[String]/////////////////////////////////////
            template<class T>
            interface TEST<27, T &> : TEST<27, T> {
            };

            template<class T>
            interface TEST<27, T &&> : TEST<27, T> {
            };

            template<class T>
            interface TEST<27, T *> : TEST<24, T> {
            };

            template<class T>
            interface TEST<27, T[]> : TEST<24, T> {
            };

            template<class T>
            interface TEST<27, T[0]> : TEST<24, T> {
            };

            template<class T, gint Size>
            interface TEST<27, T[Size]> : TEST<24, T> {
            };

            /////////////////////////////////[EQ]/////////////////////////////////////
            namespace {
                template<class T, class = decltype(FALSE_DECLARATION<T>() == FALSE_DECLARATION<T>())>
                static ALWAYS_TRUE UNSAFE_EQUAL_TESTER(gint);

                template<class ...>
                static CORE_FAST ALWAYS_FALSE UNSAFE_EQUAL_TESTER(glong);
            }

            template<class T>
            interface TEST<28, T> : decltype(UNSAFE_EQUAL_TESTER<T>(0)) {
            };

            /////////////////////////////////[LT]/////////////////////////////////////
            namespace {
                template<class T, class = decltype(FALSE_DECLARATION<T>() < FALSE_DECLARATION<T>())>
                static CORE_FAST ALWAYS_TRUE UNSAFE_LT_TESTER(gint);

                template<class ...>
                static CORE_FAST ALWAYS_FALSE UNSAFE_LT_TESTER(glong);
            }

            template<class T>
            interface TEST<29, T> : decltype(UNSAFE_LT_TESTER<T>(0)) {
            };

            /////////////////////////////////[Destructible]/////////////////////////////////////

            namespace {
                template<class T, class = decltype(FALSE_DECLARATION<T>().~T())>
                static CORE_FAST ALWAYS_TRUE UNSAFE_DESTRUCTOR_TESTER(gint);

                template<class ...>
                static CORE_FAST ALWAYS_FALSE UNSAFE_DESTRUCTOR_TESTER(glong);
            }

            template<class T>
            interface TEST<32, T> : decltype(UNSAFE_DESTRUCTOR_TESTER<T>(0)) {
            };


            /////////////////////////////////[Class]/////////////////////////////////////
            template<>
            interface MODIFY<30, gbool> : ALWAYS<Boolean> {
            };
            template<gint S>
            interface MODIFY<30, gbool[S]> : ALWAYS<BooleanArray> {
            };
            template<>
            interface MODIFY<30, gbyte> : ALWAYS<Byte> {
            };
            template<gint S>
            interface MODIFY<30, gbyte[S]> : ALWAYS<ByteArray> {
            };
            template<>
            interface MODIFY<30, gshort> : ALWAYS<Short> {
            };
            template<gint S>
            interface MODIFY<30, gshort[S]> : ALWAYS<ShortArray> {
            };
            template<>
            interface MODIFY<30, unsigned short> : ALWAYS<Short> {
            };
            template<gint S>
            interface MODIFY<30, unsigned short[S]> : ALWAYS<ShortArray> {
            };
            template<>
            interface MODIFY<30, wchar_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, wchar_t[S]> : ALWAYS<CharArray> {
            };
            template<>
            interface MODIFY<30, unsigned char> : ALWAYS<Character> {
            };
            template<>
            interface MODIFY<30, char16_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, char16_t[S]> : ALWAYS<CharArray> {
            };
            template<>
            interface MODIFY<30, char32_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, char32_t[S]> : ALWAYS<IntArray> {
            };
#ifdef __cpp_lib_char8_t
            template<>
            interface MODIFY<30, char8_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, char8_t[S]> : ALWAYS<CharArray> {
            };
#endif
            template<>
            interface MODIFY<30, gint> : ALWAYS<Integer> {
            };
            template<gint S>
            interface MODIFY<30, gint[S]> : ALWAYS<IntArray> {
            };
            template<>
            interface MODIFY<30, unsigned int> : ALWAYS<Integer> {
            };
            template<gint S>
            interface MODIFY<30, unsigned int[S]> : ALWAYS<IntArray> {
            };
            template<>
            interface MODIFY<30, long long> : ALWAYS<Long> {
            };
            template<gint S>
            interface MODIFY<30, long[S]> : ALWAYS<LongArray> {
            };
            template<>
            interface MODIFY<30, unsigned long long> : ALWAYS<Long> {
            };
            template<gint S>
            interface MODIFY<30, unsigned long long[S]> : ALWAYS<LongArray> {
            };

#if defined(__INT128__) || defined(__SIZEOF_INT128__)
            template<>
            interface MODIFY<30, __int128> : ALWAYS<Long> {
            };
            template<>
            interface MODIFY<30, unsigned __int128> : ALWAYS<Long> {
            };
#endif
            template<>
            interface MODIFY<30, long> : ALWAYS<Long> {
            };

            template<>
            interface MODIFY<30, unsigned long> : ALWAYS<Long> {
            };

            template<>
            interface MODIFY<30, gfloat> : ALWAYS<Float> {
            };

            template<>
            interface MODIFY<30, gdouble> : ALWAYS<Double> {
            };

#if defined(__FLOAT128__) || defined(__SIZEOF_FLOAT128__)
            template<>
            interface MODIFY<30, __float128> : ALWAYS<Double> {
            };
#endif
            template<>
            interface MODIFY<30, void> : ALWAYS<Void> {
            };

            template<>
            interface MODIFY<30, GENERIC_CPLEX> : ALWAYS<Complex> {
            };

            template<class T>
            interface MODIFY<30, T>
                    : CONDITIONAL<Enum<T>, TEST<16, T>::Value, typename CONDITIONAL<String, TEST<27, T>::Value, T>::Type> {
            };

            /////////////////////////////////[native]/////////////////////////////////////
            template<class T>
            interface MODIFY<31, T> : ALWAYS<T> {
            };
            template<class T>
            interface MODIFY<31, Enum<T>> : ALWAYS<T> {
            };
            template<>
            interface MODIFY<31, Void> : ALWAYS<void> {
            };
            template<>
            interface MODIFY<31, Boolean> : ALWAYS<gbool> {
            };
            template<>
            interface MODIFY<31, Byte> : ALWAYS<gbyte> {
            };
            template<>
            interface MODIFY<31, Short> : ALWAYS<gshort> {
            };
            template<>
            interface MODIFY<31, Character> : ALWAYS<gchar> {
            };
            template<>
            interface MODIFY<31, Integer> : ALWAYS<gint> {
            };
            template<>
            interface MODIFY<31, Long> : ALWAYS<glong> {
            };
            template<>
            interface MODIFY<31, Float> : ALWAYS<gfloat> {
            };

            template<>
            interface MODIFY<31, Double> : ALWAYS<gdouble> {
            };

            /////////////////////////////////[VarArgs Template]/////////////////////////////////////
            template<gint idx, class Fallback, class ...T>
            interface VA_ARGS : ALWAYS<Fallback> {
            };

            template<gint idx, class Fallback>
            interface VA_ARGS<idx, Fallback> : ALWAYS<Fallback> {
            };
            template<class Fallback, class T1, class ...TS>
            interface VA_ARGS<1, Fallback, T1, TS...> : ALWAYS<T1> {
            };
            template<class Fallback, class T1, class T2, class ...TS>
            interface VA_ARGS<2, Fallback, T1, T2, TS...> : ALWAYS<T2> {
            };
            template<class Fallback, class T1, class T2, class T3, class ...TS>
            interface VA_ARGS<3, Fallback, T1, T2, T3, TS...> : ALWAYS<T3> {
            };
            template<class Fallback, class T1, class T2, class T3, class T4, class ...TS>
            interface VA_ARGS<4, Fallback, T1, T2, T3, T4, TS...> : ALWAYS<T4> {
            };
            template<class Fallback, class T1, class T2, class T3, class T4, class T5, class ...TS>
            interface VA_ARGS<5, Fallback, T1, T2, T3, T4, T5, TS...> : ALWAYS<T5> {
            };
            template<class Fallback, class T1, class T2, class T3, class T4, class T5, class T6, class ...TS>
            interface VA_ARGS<6, Fallback, T1, T2, T3, T4, T5, T6, TS...> : ALWAYS<T6> {
            };
            template<class Fallback, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class ...TS>
            interface VA_ARGS<7, Fallback, T1, T2, T3, T4, T5, T6, T7, TS...> : ALWAYS<T7> {
            };
            template<class Fallback, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class ...TS>
            interface VA_ARGS<8, Fallback, T1, T2, T3, T4, T5, T6, T7, T8, TS...> : ALWAYS<T8> {
            };
            template<gint idx, class F, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class...TS>
            interface VA_ARGS<idx, F, T1, T2, T3, T4, T5, T6, T7, T8, TS...> : VA_ARGS<idx - 7, F, TS...> {
                CORE_STATIC_ASSERT(idx > 8, "Index out of range");
            };

            /////////////////////////////////[...]/////////////////////////////////////
            template<class...Params>
            interface FnParams {
                static CORE_FAST gint count = sizeof...(Params);
            };

            template<gint i, class Params>
            interface FnParamSelector : ALWAYS<void> {
            };

            template<gint i, class... Params>
            interface FnParamSelector<i, FnParams<Params...>> : VA_ARGS<i, void, Params...> {
            };

            template<class Signature>
            interface FnAnalyzer : FnParams<> {
            };

            template<class R, class...Params>
            interface FnAnalyzer<R(Params...)> : FnParams<Params...> {
            };

            template<class R, class...Params>
            interface FnAnalyzer<R(*)(Params...)> : FnParams<Params...> {
            };

            template<class R, class Obj, class ...Params>
            interface FnAnalyzer<R(Obj::*)(Params...)> : FnParams<Params...> {
            };

            /////////////////////////////////[Aggregate]/////////////////////////////////////
#if defined(CORE_COMPILER_MSVC) && defined(_HAS_CXX17) || ____cplusplus > 201402L
            template<class T>
            interface TEST<33, T> : CONSTANT<__is_aggregate(T)> {
            };
#endif
            /////////////////////////////////[Final class]/////////////////////////////////////
            template<class T>
            interface TEST<34, T> : CONSTANT<__is_final(T)> {
            };
            /////////////////////////////////[Final class]/////////////////////////////////////
            template<class T>
            interface TEST<35, T> : CONSTANT<__is_polymorphic(T)> {
            };
            /////////////////////////////////[Final class]/////////////////////////////////////
            template<class T>
            interface TEST<36, T> : CONSTANT<__is_empty(T)> {
            };
            /////////////////////////////////[Final class]/////////////////////////////////////
            template<class T>
            interface TEST<37, T> : CONSTANT<__is_trivial(T)> {
            };
            /////////////////////////////////[Final class]/////////////////////////////////////
            template<class T>
            interface TEST<38, T> : CONSTANT<__is_literal_type(T)> {
            };
            /////////////////////////////////[...]/////////////////////////////////////
            /////////////////////////////////[...]/////////////////////////////////////
            /////////////////////////////////[...]/////////////////////////////////////
        }

        CORE_WARNING_POP

    }
}


#endif //CORE23_TEMPLATES_H
