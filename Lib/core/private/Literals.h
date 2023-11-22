//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_LITERALS_H
#define CORE23_LITERALS_H

#include <core/Private/Templates.h>

namespace core {

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_i, 1234.5678_i
     * </pre>
     */
    CORE_ENABLE_FLOATING_LITERAL_CONSTRUCTOR(Complex, _i)

    /**
     * Enable literal Complex with forms:
     * <pre>
     * 1234_I, 1234.5678_I
     * </pre>
     */
    CORE_ENABLE_FLOATING_LITERAL_CONSTRUCTOR(Complex, _I)

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
    CORE_ENABLE_FLOATING_LITERAL_CONSTRUCTOR(Complex, i)
#endif

    /**
     * Enable literal String with forms:
     * <pre>
     * "..."_S, L"..."_S, u"..."_S, u"..."_S and U"..."_S
     * </pre>
     */
    CORE_ENABLE_STRING_LITERAL_CONSTRUCTOR(String, _S)


    /**
     * Enable literal String with forms:
     * <pre>
     * '...'_S, L'...'_S, u'...'_S, u'...'_S and U'...'_S
     * </pre>
     *
     * @deprecated this literal not supported by all version
     */
    CORE_ENABLE_CHARACTER_LITERAL_CONSTRUCTOR(String, _S)

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
    CORE_ENABLE_STRING_LITERAL_CONSTRUCTOR(String, s)
#endif

#undef CORE_ENABLE_STRING_LITERAL_CONSTRUCTOR
#undef CORE_ENABLE_INTEGER_LITERAL_CONSTRUCTOR
#undef CORE_ENABLE_FLOATING_LITERAL_CONSTRUCTOR
#undef CORE_ENABLE_CHARACTER_LITERAL_CONSTRUCTOR
#undef CORE_ENABLE_LITERAL_CONSTRUCTOR


    extern gbool operator==(const Object &o, bool b);

    extern gbool operator!=(const Object &o, bool b);

    extern gbool operator==(const Object &o, char c);

    extern gbool operator!=(const Object &o, char c);

    extern gbool operator==(const Object &o, unsigned char c);

    extern gbool operator!=(const Object &o, unsigned char c);

    extern gbool operator==(const Object &o, char16_t c);

    extern gbool operator!=(const Object &o, char16_t c);

    extern gbool operator==(const Object &o, char32_t c);

    extern gbool operator!=(const Object &o, char32_t c);

    extern gbool operator==(const Object &o, wchar_t c);

    extern gbool operator!=(const Object &o, wchar_t c);

#if __cpp_lib_char8_t >= 201907L

    extern gbool operator==(const Object &o, char8_t c);

    extern gbool operator!=(const Object &o, char8_t c);

#endif

    extern gbool operator==(const Object &o, short i);

    extern gbool operator!=(const Object &o, short i);

    extern gbool operator==(const Object &o, unsigned short i);

    extern gbool operator!=(const Object &o, unsigned short i);

    extern gbool operator==(const Object &o, int i);

    extern gbool operator!=(const Object &o, int i);

    extern gbool operator==(const Object &o, unsigned int i);

    extern gbool operator!=(const Object &o, unsigned int i);

    extern gbool operator==(const Object &o, long i);

    extern gbool operator!=(const Object &o, long i);

    extern gbool operator==(const Object &o, unsigned long i);

    extern gbool operator!=(const Object &o, unsigned long i);

    extern gbool operator==(const Object &o, long long i);

    extern gbool operator!=(const Object &o, long long i);

    extern gbool operator==(const Object &o, unsigned long long i);

    extern gbool operator!=(const Object &o, unsigned long long i);

#if defined(__SIZEOF_INT128__) || defined(__INT128__)

    extern gbool operator==(const Object &o, __int128 i);

    extern gbool operator!=(const Object &o, __int128 i);

    extern gbool operator==(const Object &o, unsigned __int128 i);

    extern gbool operator!=(const Object &o, unsigned __int128 i);

#endif

    extern gbool operator==(const Object &o, float f);

    extern gbool operator!=(const Object &o, float f);

    extern gbool operator==(const Object &o, double f);

    extern gbool operator!=(const Object &o, double f);

    extern gbool operator==(const Object &o, long double f);

    extern gbool operator!=(const Object &o, long double f);

#if defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)

    extern gbool operator==(const Object &o, __float128 f);

    extern gbool operator!=(const Object &o, __float128 f);

#endif

    extern gbool operator==(const Object &o, GENERIC_COMPLEX cplex);

    extern gbool operator!=(const Object &o, GENERIC_COMPLEX cplex);

    extern gbool operator==(const Object &o1, const Object &o2);

    extern gbool operator!=(const Object &o1, const Object &o2);

}

extern void *operator new(size_t, void *) CORE_NOTHROW;

extern void *operator new[](size_t, void *) CORE_NOTHROW;

extern void operator delete(void *, void *) CORE_NOTHROW;

extern void operator delete[](void *, void *) CORE_NOTHROW;

#endif //CORE23_LITERALS_H
