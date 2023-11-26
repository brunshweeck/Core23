//
// Created by Brunshweeck on 12/09/2023.
//

#include "Null.h"
#include <core/String.h>
#include <core/Complex.h>

namespace core {

    gbool Null::equals(const Object &o) const {
        return &o == &null && this == &null;
    }

    Object &Null::clone() const {
        return null;
    }

    String Null::toString() const {
        if (this != &null)
            return Object::toString();
        return "null";
    }

    Null null = {};

    gbool operator==(const Object &o1, const Object &o2) {
        if (&o1 == &o2)
            return true;
        return o1.equals(o2);
    }

    gbool operator!=(const Object &o1, const Object &o2) {
        return !(o1 == o2);
    }


    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_i, 1234.5678_i
     * </pre>
     */
    const Complex operator ""_i(unsigned long long img) {
        return Complex(0, (gdouble) img);
    }

    const Complex operator ""_i(long double img) {
        return Complex(0, (gdouble) img);
    }

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_I, 1234.5678_I
     * </pre>
     */
    const Complex operator ""_I(size_t img) {
        return Complex(0, (gdouble) img);
    }

    const Complex operator ""_I(long double img) {
        return Complex(0, (gdouble) img);
    }

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
    const Complex operator ""i(size_t img) {
        return Complex(0, (gdouble) img);
    }
    const Complex operator ""i(long double img) {
        return Complex(0, (gdouble) img);
    }
#endif

    /**
     * Enable literal String with forms:
     * <pre>
     * "..."_S, L"..."_S, u"..."_S, u"..."_S and U"..."_S
     * </pre>
     */
    const String operator ""_S(const char s[], size_t l) {
        return String(s, 0, l);
    }

    const String operator ""_S(const char16_t s[], size_t l) {
        return String(s, 0, l);
    }

    const String operator ""_S(const char32_t s[], size_t l) {
        return String(s, 0, l);
    }

    const String operator ""_S(const wchar_t s[], size_t l) {
        return String(s, 0, l);
    }


    /**
     * Enable literal String with forms:
     * <pre>
     * '...'_S, L'...'_S, u'...'_S, u'...'_S and U'...'_S
     * </pre>
     *
     */
    const String operator ""_S(char c) {
        return String(&c, 0, 1);
    }

    const String operator ""_S(char16_t c) {
        return String(&c, 0, 1);
    }

    const String operator ""_S(char32_t c) {
        return String(&c, 0, 1);
    }

    const String operator ""_S(wchar_t c) {
        return String(&c, 0, 1);
    }

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
    const String operator ""s(const char s[], size_t l) {
        return String(s, 0, l);
    }

    const String operator ""s(const char16_t s[], size_t l) {
        return String(s, 0, l);
    }

    const String operator ""s(const char32_t s[], size_t l) {
        return String(s, 0, l);
    }

    const String operator ""s(const wchar_t s[], size_t l) {
        return String(s, 0, l);
    }

#endif

} // core

