//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_LITERALS_H
#define CORE23_LITERALS_H

#include <stddef.h>
#include "Templates.h"

namespace core {

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_i, 1234.5678_i
     * </pre>
     */
    extern Complex operator ""_i(unsigned long long);

    extern Complex operator ""_i(long double);

    /**
     * Enable literal String with forms:
     * <pre>
     * "..."_S, L"..."_S, u"..."_S, u"..."_S and U"..."_S
     * </pre>
     */
    extern String operator ""_S(const char[], size_t);

    extern String operator ""_S(const char16_t[], size_t);

    extern String operator ""_S(const char32_t[], size_t);

    extern String operator ""_S(const wchar_t[], size_t);


    /**
     * Enable literal String with forms:
     * <pre>
     * '...'_S, L'...'_S, u'...'_S, u'...'_S and U'...'_S
     * </pre>
     *
     */
    extern String operator ""_S(char);

    extern String operator ""_S(char16_t);

    extern String operator ""_S(char32_t);

    extern String operator ""_S(wchar_t);

    extern gbool operator==(const Object &, const Object &);

    extern gbool operator!=(const Object &, const Object &);
}

extern GENERIC_PTR operator new(size_t, GENERIC_PTR) CORE_NOTHROW;

extern GENERIC_PTR operator new[](size_t, GENERIC_PTR) CORE_NOTHROW;

extern void operator delete(GENERIC_PTR, GENERIC_PTR) CORE_NOTHROW;

extern void operator delete[](GENERIC_PTR, GENERIC_PTR) CORE_NOTHROW;

// no inline, required by [replacement.functions]/3
extern GENERIC_PTR operator new(size_t sz);

// no inline, required by [replacement.functions]/3
extern GENERIC_PTR operator new[](size_t sz);

extern void operator delete(GENERIC_PTR ptr) CORE_NOTHROW;

extern void operator delete(GENERIC_PTR ptr, size_t size) CORE_NOTHROW;

extern void operator delete[](GENERIC_PTR ptr) CORE_NOTHROW;

#endif //CORE23_LITERALS_H
