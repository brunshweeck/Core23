//
// Created by Brunshweeck on 11/09/2023 17:57.
// Copyright (c) 2023
//

#ifndef CORE23_TEMPLATES_H
#define CORE23_TEMPLATES_H

#include <core/Private/Generics.h>

namespace core {

    namespace native {

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        namespace Templates {

            template<gbool X>
            interface CONSTANT {
                static CORE_FAST gbool Value = X;
            };

            template<class Alias>
            interface ALWAYS {
                CORE_ALIAS(Type, Alias);
            };

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
            interface TEST<2, T const> : ALWAYS_TRUE {
            };

            template<class T>
            interface MODIFY<2, T> : ALWAYS<T const> {
            };

            template<class T>
            interface MODIFY<-2, T const> : ALWAYS<T> {
            };



            /////////////////////////////////[Volatile]/////////////////////////////////////

            template<class T>
            interface TEST<3, T volatile> : ALWAYS_TRUE {
            };

            template<class T>
            interface MODIFY<3, T> : ALWAYS<T volatile> {
            };

            template<class T>
            interface MODIFY<-3, T volatile> : ALWAYS<T> {
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

            template<class T>
            interface MODIFY<4, T> : ALWAYS<T &> {
            };

            template<>
            interface MODIFY<4, void> : ALWAYS<void> {
            };

            template<class T>
            interface MODIFY<44, T> : ALWAYS<T &&> {
            };

            template<>
            interface MODIFY<44, void> : ALWAYS<void> {
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
            interface TEST<5, T &> : TEST<5, T> {
            };
            template<class T>
            interface TEST<5, T &&> : TEST<5, T> {
            };

            template<class T>
            interface TEST<5, T *> : ALWAYS_TRUE {
            };

            template<class T>
            interface MODIFY<5, T> : ALWAYS<T *> {
            };

            template<class T>
            interface MODIFY<5, T &> : MODIFY<5, T> {
            };

            template<class T>
            interface MODIFY<5, T &&> : MODIFY<5, T> {
            };

            template<class T>
            interface MODIFY<5, T[]> : ALWAYS<typename MODIFY<5, T>::Type *> {
            };

            template<class T, gint Size>
            interface MODIFY<5, T[Size]> : ALWAYS<typename MODIFY<5, T>::Type *> {
            };

            template<class T>
            interface MODIFY<-5, T *> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-5, T *&> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-5, T *&&> : ALWAYS<T> {
            };
            /////////////////////////////////[ConstVol]/////////////////////////////////////
            template<class T>
            interface MODIFY<6, T> : ALWAYS<T const volatile> {
            };

            template<class T>
            interface MODIFY<-6, T const> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-6, T volatile> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-6, T const volatile> : ALWAYS<T> {
            };
            /////////////////////////////////[ConstRef]/////////////////////////////////////
            template<class T>
            interface MODIFY<7, T> : ALWAYS<const T &> {
            };

            template<>
            interface MODIFY<7, void> : ALWAYS<void const> {
            };

            template<class T>
            interface MODIFY<-7, T &> : MODIFY<-7, T> {
            };

            template<class T>
            interface MODIFY<-7, T &&> : MODIFY<-7, T> {
            };

            template<class T>
            interface MODIFY<-7, T const> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<77, T> : ALWAYS<const T &&> {
            };

            template<>
            interface MODIFY<77, void> : ALWAYS<void const> {
            };

            template<class T>
            interface MODIFY<-77, T &> : MODIFY<-77, T> {
            };

            template<class T>
            interface MODIFY<-77, T &&> : MODIFY<-77, T> {
            };

            template<class T>
            interface MODIFY<-77, T const> : ALWAYS<T> {
            };

            /////////////////////////////////[ConstVolRef]/////////////////////////////////////

            template<>
            interface MODIFY<8, void> : ALWAYS<void const volatile> {
            };

            template<class T>
            interface MODIFY<8, T> : ALWAYS<T const volatile &> {
            };

            template<class T>
            interface MODIFY<8, T &> : ALWAYS<T const volatile &> {
            };

            template<class T>
            interface MODIFY<8, T &&> : ALWAYS<T const volatile &> {
            };

            template<class T>
            interface MODIFY<8, T const> : ALWAYS<T const volatile &> {
            };

            template<class T>
            interface MODIFY<8, T volatile> : ALWAYS<T const volatile &> {
            };

            template<class T>
            interface MODIFY<8, T const volatile> : ALWAYS<T const volatile &> {
            };

            template<class T>
            interface MODIFY<-8, T &> : MODIFY<-8, T> {
            };

            template<class T>
            interface MODIFY<-8, T &&> : MODIFY<-8, T> {
            };

            template<class T>
            interface MODIFY<-8, T const> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-8, T volatile> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-8, T const volatile> : ALWAYS<T> {
            };

            template<>
            interface MODIFY<88, void> : ALWAYS<void const volatile> {
            };

            template<class T>
            interface MODIFY<88, T> : ALWAYS<T const volatile &&> {
            };

            template<class T>
            interface MODIFY<88, T &> : ALWAYS<T const volatile &&> {
            };

            template<class T>
            interface MODIFY<88, T &&> : ALWAYS<T const volatile &&> {
            };

            template<class T>
            interface MODIFY<88, T const> : ALWAYS<T const volatile &&> {
            };

            template<class T>
            interface MODIFY<88, T volatile> : ALWAYS<T const volatile &&> {
            };

            template<class T>
            interface MODIFY<88, T const volatile> : ALWAYS<T const volatile &&> {
            };

            template<class T>
            interface MODIFY<-88, T &> : ALWAYS<T &> {
            };

            template<class T>
            interface MODIFY<-88, T &&> : MODIFY<-8, T> {
            };

            template<class T>
            interface MODIFY<-88, T const> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-88, T volatile> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-88, T const volatile> : ALWAYS<T> {
            };

            /////////////////////////////////[Array]/////////////////////////////////////

            template<class T>
            interface TEST<9, T &> : TEST<9, T> {
            };
            template<class T>
            interface TEST<9, T &&> : TEST<9, T> {
            };
            template<class T>
            interface TEST<9, T const> : TEST<9, T> {
            };
            template<class T>
            interface TEST<9, T volatile> : TEST<9, T> {
            };
            template<class T>
            interface TEST<9, T const volatile> : TEST<9, T> {
            };

            template<class T>
            interface TEST<9, T[]> : ALWAYS_TRUE {
            };

            template<class T>
            interface TEST<9, T[0]> : ALWAYS_TRUE {
            };

            template<class T, gint Size>
            interface TEST<9, T[Size]> : ALWAYS_TRUE {
            };

            template<class T, gint Size>
            interface MODIFY<9, T, Size> : ALWAYS<T[Size & 0x7FFFFFFF]> {
            };

            template<class T>
            interface MODIFY<9, T, 0> : ALWAYS<T[0]> {
            };

            template<class T>
            interface MODIFY<9, T, -1> : ALWAYS<T[]> {
            };

            template<class T>
            interface MODIFY<-9, T[]> : ALWAYS<T> {
            };

            template<class T>
            interface MODIFY<-9, T[0]> : ALWAYS<T> {
            };

            template<class T, gint Size>
            interface MODIFY<-9, T[Size]> : ALWAYS<T> {
            };

            template<class T, gint Size>
            interface MODIFY<-9, T(&)[Size]> : ALWAYS<T &> {
            };

            template<class T>
            interface MODIFY<-9, T(&)[0]> : ALWAYS<T &> {
            };

            template<class T>
            interface MODIFY<-9, T(&&)[0]> : ALWAYS<T &> {
            };

            template<class T, gint Size>
            interface MODIFY<-9, T(&&)[Size]> : ALWAYS<T &&> {
            };

            /////////////////////////////////[Template]/////////////////////////////////////

            template<class T>
            interface TEST<11, T &> : TEST<11, T> {
            };
            template<class T>
            interface TEST<11, T &&> : TEST<11, T> {
            };
            template<class T>
            interface TEST<11, T const> : TEST<11, T> {
            };
            template<class T>
            interface TEST<11, T volatile> : TEST<11, T> {
            };
            template<class T>
            interface TEST<11, T const volatile> : TEST<11, T> {
            };
            template<template<class ...> class T, class ...Aliases>
            interface TEST<11, T<Aliases...>> : ALWAYS_TRUE {
            };

            /////////////////////////////////[function]/////////////////////////////////////

            template<class T>
            interface TEST<12, T> : TEST<1, T, T const> {
            };

            template<class R>
            interface TEST<12, R(*)(...)> : ALWAYS_TRUE {
            };

            template<class R>
            interface TEST<12, R(*)()> : ALWAYS_TRUE {
            };

            template<class R, class...Aliases>
            interface TEST<12, R(*)(Aliases...)> : ALWAYS_TRUE {
            };

            /////////////////////////////////[Class Member]/////////////////////////////////////

            template<class T, class U>
            interface TEST<13, T U::*> : ALWAYS_TRUE {
            };

            /////////////////////////////////[function Member]/////////////////////////////////////

            template<class T, class U>
            interface TEST<14, T U::*> : TEST<12, T> {
            };

            template<class T, class U, class ...Aliases>
            interface TEST<14, T(U::*)(Aliases...)> : ALWAYS_TRUE {
            };

            /////////////////////////////////[Abstract]/////////////////////////////////////

            template<class T>
            interface TEST<15, T> : CONSTANT<__is_abstract(T)> {
            };

            template<>
            interface TEST<15, Void> : ALWAYS_FALSE {
            };

            /////////////////////////////////[Enum]/////////////////////////////////////

            template<class T>
            interface TEST<16, T> : CONSTANT<__is_enum(T)> {
            };

            template<>
            interface TEST<16, Void> : ALWAYS_FALSE {
            };

            /////////////////////////////////[Class]/////////////////////////////////////

            template<class T>
            interface TEST<17, T> : CONSTANT<__is_class(T)> {
            };

            template<>
            interface TEST<17, Void> : ALWAYS_TRUE {
            };

            /////////////////////////////////[Constructible]/////////////////////////////////////

            template<class T, class...Aliases>
            interface TEST<18, T, Aliases...> : CONSTANT<__is_constructible(T, Aliases...)> {
            };

            /////////////////////////////////[Assignable]/////////////////////////////////////

            template<class T, class U>
            interface TEST<19, T, U> : CONSTANT<__is_assignable(T, U)> {
            };

            /////////////////////////////////[Super]/////////////////////////////////////

            template<class T, class U>
            interface TEST<20, T, U> : CONSTANT<__is_base_of(T, U)> {
            };

            template<class T>
            interface TEST<20, T, Void> : ALWAYS_FALSE {
            };

            template<class T>
            interface TEST<20, Void, T> : ALWAYS_FALSE {
            };

            template<>
            interface TEST<20, Void, Void> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Void> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Object> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Boolean> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Byte> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Integer> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Long> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Float> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Double> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Complex> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, String> : ALWAYS_TRUE {
            };

            template<>
            interface TEST<20, Object, Character> : ALWAYS_TRUE {
            };

            template<class E>
            interface TEST<20, Object, Enum<E>> : ALWAYS_TRUE {
            };



            /////////////////////////////////[Conditional]/////////////////////////////////////

            template<class T, gbool>
            interface CONDITIONAL_STRICT : ALWAYS_FALSE {
            };

            template<class T>
            interface CONDITIONAL_STRICT<T, true> : ALWAYS<T> {
            };

            template<class T, gbool, class U>
            interface CONDITIONAL : ALWAYS<U> {
            };

            template<class T, class U>
            interface CONDITIONAL<T, true, U> : ALWAYS<T> {
            };

            // END
        }

    }
}

namespace {

    CORE_ALIAS(gbyte,
               typename core::native::Templates::CONDITIONAL<
                       core::native::Generics::GENERIC_BYTE, ,
               !core::native::Templates::TEST<0, char, signed char>::Value,
               core::native::Generics::GENERIC_BYTE_ALT > ::Type);
    CORE_ALIAS(glong,
               typename core::native::Templates::CONDITIONAL<
                       core::native::Generics::GENERIC_INT64, ,
               sizeof(core::native::Generics::GENERIC_INT64) == 8,
               core::native::Generics::GENERIC_INT64_ALT > ::Type);
    CORE_ALIAS(gdouble,
               typename core::native::Templates::CONDITIONAL<
                       core::native::Generics::GENERIC_FLOAT64, ,
               sizeof(core::native::Generics::GENERIC_FLOAT64) == 8,
               core::native::Generics::GENERIC_FLOAT64_ALT > ::Type);


#ifdef CORE_COMPILER_MSVC
#ifdef _DComplex
    CORE_ALIAS(GENERIC_COMPLEX, _DComplex);
#else
    CORE_ALIAS(GENERIC_COMPLEX, struct _C_double_complex);
#endif
#else
    CORE_ALIAS(GENERIC_COMPLEX, _Complex double);
#endif


    CORE_STATIC_ASSERT(
            sizeof(gbyte) == sizeof(gbool) &&
            sizeof(glong) == sizeof(gdouble) &&
            sizeof(glong) == (sizeof(gint) << 1),
            "This compiler not supported. On architecture:" CORE_ARCH);

}

namespace core {

    namespace native {

        namespace Templates {

            namespace {

                template<class T, class U = T &&>
                static U UNSAFE_DECLARATION(gint);

                template<class T>
                static T UNSAFE_DECLARATION(glong);

                template<class T>
                static decltype(UNSAFE_DECLARATION<T>(0)) FALSE_DECLARATION();

            }



            /////////////////////////////////[Complete]/////////////////////////////////////

            template<class T>
            interface TEST<10, T &> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, T &&> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, T const> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, T volatile> : TEST<10, T> {
            };
            template<class T>
            interface TEST<10, T const volatile> : TEST<10, T> {
            };

#if __has_builtin(__is_complete_type)
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
            interface TEST<25, Generics::GENERIC_BOOLEAN> : ALWAYS_TRUE {
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
                static CORE_FAST gbool Value = TEST<1, Type, ALWAYS_FALSE>::Value == false;
            };

            template<class T, class U, class V, class ...W>
            interface TEST<26, T U::*, V, W...> {
                CORE_ALIAS(V2, typename MODIFY<-4,, V > ::Type);
                CORE_ALIAS(Type1, decltype(UNSAFE_METHOD_TESTER<T U::*, V2, W...>(0)));
                CORE_ALIAS(Type2, decltype(UNSAFE_MEMBER_TESTER<T U::*, V2, W...>(0)));
                static CORE_FAST gbool Value1 = TEST<1, Type1, ALWAYS_FALSE>::Value == false;
                static CORE_FAST gbool Value2 = TEST<1, Type2, ALWAYS_FALSE>::Value == false;
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
            interface MODIFY<30, gbool[S]> : ALWAYS<primitive::BooleanArray> {
            };
            template<>
            interface MODIFY<30, gbyte> : ALWAYS<Byte> {
            };
            template<gint S>
            interface MODIFY<30, gbyte[S]> : ALWAYS<primitive::ByteArray> {
            };
            template<>
            interface MODIFY<30, gshort> : ALWAYS<Short> {
            };
            template<gint S>
            interface MODIFY<30, gshort[S]> : ALWAYS<primitive::ShortArray> {
            };
            template<>
            interface MODIFY<30, unsigned short> : ALWAYS<Short> {
            };
            template<gint S>
            interface MODIFY<30, unsigned short[S]> : ALWAYS<primitive::ShortArray> {
            };
            template<>
            interface MODIFY<30, wchar_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, wchar_t[S]> : ALWAYS<primitive::CharArray> {
            };
            template<>
            interface MODIFY<30, unsigned char> : ALWAYS<Character> {
            };
            template<>
            interface MODIFY<30, char16_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, char16_t[S]> : ALWAYS<primitive::CharArray> {
            };
            template<>
            interface MODIFY<30, char32_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, char32_t[S]> : ALWAYS<primitive::IntArray> {
            };
#ifdef __cpp_lib_char8_t
            template<>
            interface MODIFY<30, char8_t> : ALWAYS<Character> {
            };
            template<gint S>
            interface MODIFY<30, char8_t[S]> : ALWAYS<primitive::CharArray> {
            };
#endif
            template<>
            interface MODIFY<30, gint> : ALWAYS<Integer> {
            };
            template<gint S>
            interface MODIFY<30, gint[S]> : ALWAYS<primitive::IntArray> {
            };
            template<>
            interface MODIFY<30, unsigned int> : ALWAYS<Integer> {
            };
            template<gint S>
            interface MODIFY<30, unsigned int[S]> : ALWAYS<primitive::IntArray> {
            };
            template<>
            interface MODIFY<30, long long> : ALWAYS<Long> {
            };
            template<gint S>
            interface MODIFY<30, long[S]> : ALWAYS<primitive::LongArray> {
            };
            template<>
            interface MODIFY<30, unsigned long long> : ALWAYS<Long> {
            };
            template<gint S>
            interface MODIFY<30, unsigned long long[S]> : ALWAYS<primitive::LongArray> {
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
            interface MODIFY<30, long> : CONDITIONAL<Integer, sizeof(long) == 4, Long> {
            };

            template<>
            interface MODIFY<30, unsigned long> : CONDITIONAL<Integer, sizeof(long) == 4, Long> {
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
            interface MODIFY<30, GENERIC_COMPLEX> : ALWAYS<Complex> {
            };

            template<class T>
            interface MODIFY<30, T>
                    : CONDITIONAL<Enum<T>, TEST<16, T>::Value, typename CONDITIONAL<String, TEST<27, T>::Value, T>::Type> {
            };

            /////////////////////////////////[primitive]/////////////////////////////////////
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
            /////////////////////////////////[...]/////////////////////////////////////
            /////////////////////////////////[...]/////////////////////////////////////
        }

        CORE_WARNING_POP

        namespace Templates {
            /////////////////////////////////[User Defined Literal]/////////////////////////////////////
#define CORE_ENABLE_STRING_LITERAL_CONSTRUCTOR(class, suffix) \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, const char[],, size_t); \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, const char16_t[],, size_t); \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, const char32_t[],, size_t); \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, const wchar_t[],, size_t);

#define CORE_ENABLE_CHARACTER_LITERAL_CONSTRUCTOR(class, suffix) \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, char); \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, char16_t); \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, char32_t); \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, wchar_t);

#define CORE_ENABLE_INTEGER_LITERAL_CONSTRUCTOR(class, suffix) \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, unsigned long long);

#define CORE_ENABLE_FLOATING_LITERAL_CONSTRUCTOR(class, suffix) \
        CORE_ENABLE_INTEGER_LITERAL_CONSTRUCTOR(class, suffix) \
        CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, long double);

        }
    }
}


#endif //CORE23_TEMPLATES_H
