//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_LITERALS_H
#define CORE23_LITERALS_H

#include <core/private/Templates.h>
#include <stddef.h>

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
    extern gbool operator==(const Object &, const String &);

    extern gbool operator!=(const Object &, const Object &);
    extern gbool operator!=(const Object &, const String &);
}

extern GENERIC_PTR operator new(size_t sizeInBytes, GENERIC_PTR cAddress) CORE_NOTHROW;

extern GENERIC_PTR operator new[](size_t sizeInBytes, GENERIC_PTR cAddress) CORE_NOTHROW;

extern void operator delete(GENERIC_PTR sizeInBytes, GENERIC_PTR cAddress) CORE_NOTHROW;

extern void operator delete[](GENERIC_PTR sizeInBytes, GENERIC_PTR cAddress) CORE_NOTHROW;
//
//// no inline, required by [replacement.functions]/3
extern GENERIC_PTR operator new(size_t sizeInBytes);
//
//// no inline, required by [replacement.functions]/3
extern GENERIC_PTR operator new[](size_t sizeInBytes);
//
extern void operator delete(GENERIC_PTR cAddress) CORE_NOTHROW;
//
extern void operator delete(GENERIC_PTR cAddress, size_t sizeInBytes) CORE_NOTHROW;
//
extern void operator delete[](GENERIC_PTR cAddress) CORE_NOTHROW;

extern void operator delete[](GENERIC_PTR cAddress, size_t sizeInBytes) CORE_NOTHROW;
//


#ifdef UNDERFLOW
#undef UNDERFLOW
#endif
#ifdef OVERFLOW
#undef OVERFLOW
#endif

#ifdef max // is predefined on MSVC compiler
#undef max
#undef min
#endif

#endif //CORE23_LITERALS_H
