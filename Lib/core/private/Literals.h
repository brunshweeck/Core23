//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_LITERALS_H
#define CORE23_LITERALS_H

#include <core/private/Templates.h>

namespace core {

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_i, 1234.5678_i
     * </pre>
     */
    extern const Complex operator ""_i(size_t);

    extern const Complex operator ""_i(long double);

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_I, 1234.5678_I
     * </pre>
     */
    extern const Complex operator ""_I(size_t);

    extern const Complex operator ""_I(long double);

#if __cpp_lib_complex_udls < 201309L

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234i, 1234.5678i
     * </pre>
     *
     * @deprecated this literal cause ambiguity
     * with standard complex types (_Complex [double, long double,...] in GNUC; [_L, _D, ...]complex in MSVC;
     * and std::complex)
     */
    extern const Complex operator ""i(size_t);
    extern const Complex operator ""i(long double);
#endif

    /**
     * Enable literal String with forms:
     * <pre>
     * "..."_S, L"..."_S, u"..."_S, u"..."_S and U"..."_S
     * </pre>
     */
    extern const String operator ""_S(const char[], size_t);

    extern const String operator ""_S(const char16_t[], size_t);

    extern const String operator ""_S(const char32_t[], size_t);

    extern const String operator ""_S(const wchar_t[], size_t);


    /**
     * Enable literal String with forms:
     * <pre>
     * '...'_S, L'...'_S, u'...'_S, u'...'_S and U'...'_S
     * </pre>
     *
     */
    extern const String operator ""_S(char);

    extern const String operator ""_S(char16_t);

    extern const String operator ""_S(char32_t);

    extern const String operator ""_S(wchar_t);

#if __cpp_lib_string_udls < 201304L

    /**
     * Enable literal String with forms:
     * <pre>
     * "..."s, L"..."s, u"..."s, u"..."s and U"..."s
     * </pre>
     *
     * @deprecated this literal cause ambiguity
     * with standard string type (std::[w|u16|u32]string)
     */
    extern const String operator ""s(const char[], size_t);
    extern const String operator ""s(const char16_t[], size_t);
    extern const String operator ""s(const char32_t[], size_t);
    extern const String operator ""s(const wchar_t[], size_t);
#endif


    extern gbool operator==(const Object &, const Object &);

    extern gbool operator!=(const Object &, const Object &);
}

extern void *operator new(size_t, void *) CORE_NOTHROW;

extern void *operator new[](size_t, void *) CORE_NOTHROW;

extern void operator delete(void *, void *) CORE_NOTHROW;

extern void operator delete[](void *, void *) CORE_NOTHROW;

#endif //CORE23_LITERALS_H
