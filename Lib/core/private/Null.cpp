//
// Created by Brunshweeck on 12/09/2023.
//

#include "Null.h"
#include <core/String.h>
#include <core/Complex.h>
#include <core/Integer.h>
#include <core/IllegalArgumentException.h>
#include <core/native/CharArray.h>
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    gbool Null::equals(const Object &o) const { return &o == &null && this == &null; }

    Object &Null::clone() const { return INSTANCE; }

    String Null::toString() const { return "null"; }

    Null Null::INSTANCE = {};

    Null &null = Null::INSTANCE;

    gbool operator==(const Object &o1, const Object &o2) {
        if(&o1 == &o2)
            return true;
        if(&o1 == null)
            return false;
        if(&o2 == null)
            return false;
        return o1.equals(o2);
    }

    gbool operator==(const Object &o1, const String &s) {
        return o1 == (const Object &) s;
    }

    gbool operator!=(const Object &o1, const Object &o2) {
        return !(o1 == o2);
    }

    gbool operator!=(const Object &o1, const String &s) {
        return !(o1 == s);
    }

    CORE_WARNING_PUSH
    CORE_WARNING_DISABLE_UDL

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
        if (s == null) {
            IllegalArgumentException("Illegal literal string").throws(__xtrace());
        }
        if (l > Integer::MAX_VALUE) {
            IllegalArgumentException("Literal string length exceed integer range").throws(__xtrace());
        }
        if (l == 0)
            return {};
        return String(s, 0, (gint) l);
    }

    String operator ""_S(const char16_t s[], size_t l) {
        if (s == null) {
            IllegalArgumentException("Illegal literal string").throws(__xtrace());
        }
        if (l > Integer::MAX_VALUE) {
            IllegalArgumentException("Literal string length exceed integer range").throws(__xtrace());
        }
        if (l == 0)
            return {};
        gint const length = (gint) l;
        CharArray chars = CharArray(length);
        for (int i = 0; i < length; i += 1) {
            chars[i] = s[i];
        }
        return String(chars);
    }

    String operator ""_S(const char32_t s[], size_t l) {
        if (s == null) {
            IllegalArgumentException("Illegal literal string").throws(__xtrace());
        }
        if (l > Integer::MAX_VALUE) {
            IllegalArgumentException("Literal string length exceed integer range").throws(__xtrace());
        }
        if (l == 0)
            return {};
        gint length = 0;
        size_t i;
        for (i = 0; i < l && length >= 0; ++i) {
            length += Character::charCount((gint) s[i]);
        }
        if (i < l) {
            IllegalArgumentException("Literal string length exceed integer range").throws(__xtrace());
        }
        CharArray chars = CharArray(length);
        i = 0;
        for (int j = 0; j < length; ++j) {
            gint const cp = (gint) s[i++];
            if (cp > Character::MAX_VALUE) {
                chars[j++] = Character::highSurrogate(cp);
                chars[j] = Character::lowSurrogate(cp);
            } else {
                chars[j] = (gchar) cp;
            }
        }
        return String(chars);
    }

    String operator ""_S(const wchar_t s[], size_t l) {
        CORE_ALIAS(LargeToUnicode, , typename Class<char16_t>::If<sizeof(wchar_t) == 2, char32_t>);
        return operator ""_S((const LargeToUnicode *) s, l);
    }


    /**
     * Enable literal String with forms:
     * <pre>
     * '...'_S, L'...'_S, u'...'_S, u'...'_S and U'...'_S
     * </pre>
     *
     */
    String operator ""_S(char c) {
        return String(CharArray::of(c));
    }

    String operator ""_S(char16_t c) {
        return String(CharArray::of(c));
    }

    String operator ""_S(char32_t c) {
        gint const cp = (gint) c;
        if (cp > Character::MAX_VALUE) {
            return String(CharArray::of(Character::highSurrogate(cp), Character::lowSurrogate(cp)));
        }
        return String(CharArray::of((gchar) cp));
    }

    String operator ""_S(wchar_t c) {
        CORE_ALIAS(LargeToUnicode, , typename Class<char16_t>::If<sizeof(wchar_t) == 2, char32_t>);
        return operator ""_S((LargeToUnicode) c);
    }

    CORE_WARNING_POP
} // core

using namespace core;
using namespace native;

GENERIC_PTR operator new(size_t sizeInBytes) {
    if (sizeInBytes > Long::MAX_VALUE)
        sizeInBytes = Long::MAX_VALUE - 8;
    return (GENERIC_PTR) Unsafe::allocateMemory((glong) sizeInBytes);
}

GENERIC_PTR operator new[](size_t sizeInBytes) {
    return operator new(sizeInBytes);
}

void operator delete(GENERIC_PTR cAddress) CORE_NOTHROW {
    Unsafe::freeMemory((glong) cAddress);
}

void operator delete(GENERIC_PTR cAddress, size_t sizeInBytes) CORE_NOTHROW {
    CORE_IGNORE(sizeInBytes);
    operator delete(cAddress);
}

void operator delete[](GENERIC_PTR cAddress) CORE_NOTHROW {
    return operator delete(cAddress);
}

void operator delete[](GENERIC_PTR cAddress, size_t sizeInBytes) CORE_NOTHROW {
    CORE_IGNORE(sizeInBytes);
    return operator delete(cAddress, sizeInBytes);
}
