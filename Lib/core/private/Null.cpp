//
// Created by Brunshweeck on 12/09/2023.
//

#include "Null.h"
#include <core/String.h>
#include <core/Complex.h>

namespace core {

    gbool Null::equals(const Object &o) const { return &o == &null && this == &null; }

    Object &Null::clone() const { return INSTANCE; }

    String Null::toString() const { return "null"; }

    Null Null::INSTANCE = {};

    Null& null = Null::INSTANCE;

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
    Complex operator ""_i(unsigned long long img) { return Complex(0, (gdouble) img); }

    Complex operator ""_i(long double img) { return Complex(0, (gdouble) img); }


    /**
     * Enable literal String with forms:
     * <pre>
     * "..."_S, L"..."_S, u"..."_S, u"..."_S and U"..."_S
     * </pre>
     */
    String operator ""_S(const char s[], size_t l) {
        return String(s, 0, l > Integer::MAX_VALUE ? Integer::MAX_VALUE : (gint) l);
    }

    String operator ""_S(const char16_t s[], size_t l) {
        return String(s, 0, l > Integer::MAX_VALUE ? Integer::MAX_VALUE : (gint) l);
    }

    String operator ""_S(const char32_t s[], size_t l) {
        return String(s, 0, l > Integer::MAX_VALUE ? Integer::MAX_VALUE : (gint) l);
    }

    String operator ""_S(const wchar_t s[], size_t l) {
        return String(s, 0, l > Integer::MAX_VALUE ? Integer::MAX_VALUE : (gint) l);
    }


    /**
     * Enable literal String with forms:
     * <pre>
     * '...'_S, L'...'_S, u'...'_S, u'...'_S and U'...'_S
     * </pre>
     *
     */
    String operator ""_S(char c) {
        return String(&c, 0, 1);
    }

    String operator ""_S(char16_t c) {
        return String(&c, 0, 1);
    }

    String operator ""_S(char32_t c) {
        return String(&c, 0, 1);
    }

    String operator ""_S(wchar_t c) {
        return String(&c, 0, 1);
    }

} // core

