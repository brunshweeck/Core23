//
// Created by Brunshweeck on 11/09/2023 17:59.
// Copyright (c) 2023
//

#ifndef CORE23_GENERICS_H
#define CORE23_GENERICS_H

#include "Compiler.h"

namespace core {

    namespace io {}
    namespace charset {}
    namespace math {}
    namespace net {}
    namespace util {}
    namespace sql {}
    namespace text {}
    namespace time {}
    namespace util {}
    namespace regex {}
    namespace random {}
    namespace gui {}
    namespace native {
        class BooleanArray;

        class ByteArray;

        class ShortArray;

        class CharArray;

        class IntArray;

        class LongArray;

        class FloatArray;

        class DoubleArray;

        class StringArray;

        class ReferenceArray;
    }

    using native::BooleanArray;
    using native::ByteArray;
    using native::ShortArray;
    using native::CharArray;
    using native::IntArray;
    using native::LongArray;
    using native::FloatArray;
    using native::DoubleArray;
    using native::ReferenceArray;
    using native::StringArray;

    namespace native {


#ifndef CORE_ALIAS
#define CORE_ALIAS(alias, target, ...) using alias = target __VA_ARGS__
#define CORE_USING(namespace, Element, ...) using namespace::target __VA_ARGS__
#endif //CORE_ALIAS

#ifndef CORE_STATIC_ASSERT
#define CORE_STATIC_ASSERT(expr, message) static_assert(expr, message)
#endif //CORE_STATIC_ASSERT

#ifndef CORE_FAST
#define CORE_FAST CORE_CONSTEXPR
#endif //CORE_FAST

#ifndef interface
#define interface struct
#endif //interface

#ifndef CORE_CAST
#define CORE_CAST(class, expr) ((class)expr)
#define CORE_DYN_CAST(class, expr) dynamic_cast<class>(expr)
#endif //CORE_CAST

#ifndef CORE_IGNORE
#define CORE_IGNORE(expr) CORE_CAST(void, expr)
#endif //CORE_IGNORE


#ifndef CORE_FILE

#ifndef __FILE__
#define CORE_FILE __FILE_NAME__

#else
#define CORE_FILE __FILE__
#endif //__FILE__

#endif

#ifndef CORE_LINE
#ifdef __LINE__
#define CORE_LINE __LINE__

#else
#define CORE_LINE 0
#endif //__LINE__

#endif

#if !defined(__cplusplus)
#error "This platform require C++11 or later"
#endif

        extern "C" {

        typedef signed char GENERIC_INT8;
        typedef unsigned char GENERIC_UINT8;
        typedef signed short int GENERIC_INT16;
        typedef unsigned short int GENERIC_UINT16;
        typedef signed int GENERIC_INT32;
        typedef unsigned int GENERIC_UINT32;
#if __WORDSIZE == 64
        typedef signed long int GENERIC_INT64;
        typedef unsigned long int GENERIC_UINT64;
#else
        typedef signed long long int GENERIC_INT64;
        typedef unsigned long long int GENERIC_UINT64;
#endif //
        typedef float GENERIC_FLT32;
        typedef double GENERIC_FLT64;
        typedef char16_t GENERIC_CHR16;
        typedef char32_t GENERIC_CHR32;
        typedef bool GENERIC_BOOL;
        typedef void *GENERIC_PTR;
        typedef std::exception GENERIC_THROWABLE;

        }

#define CORE_ENABLE_IMPLICIT_CAST(class, op, ...)   operator class () __VA_ARGS__ { return CORE_CAST(class, op); }

#ifdef CORE_SHOW_TRACE_MODULE
#ifndef CORE_TRACE_MODULE
#define CORE_TRACE_MODULE "", "" // on format: [module-name], [module-version]
#endif
#define __trace(classname) Trace(CORE_GENERIC_MODULE CORE_GENERIC_VERSION classname, &CORE_FUNCTION[0], &CORE_FILE[0], CORE_LINE)
#define __ftrace(classname) Trace(CORE_GENERIC_MODULE CORE_GENERIC_VERSION classname, &CORE_FUNCTION_SIGNATURE[0], &CORE_FILE[0], CORE_LINE)
#else
// trace represent a simple function/method trace (trace with function name).
#define __trace(...) Trace(__VA_ARGS__, CORE_FUNCTION, CORE_FILE, CORE_LINE)

// ftrace represent a full function/method trace (trace with function signature).
#define __ftrace(...) Trace(__VA_ARGS__, CORE_FUNCTION_SIGNATURE, CORE_FILE, CORE_LINE)

#endif //CORE_SHOW_TRACE_MODULE

#ifdef CORE_XCOMPILER_UDL
#define CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, params, ...) extern const class operator"" ## suffix (params __VA_ARGS__)

#else
#define CORE_ENABLE_GENERIC_LITERAL(class, suffix, params,...)
#endif //CORE_XCOMPILER_UDL

#ifndef CORE_ASSERT
#define CORE_ASSERT(expr, class) { if(!(expr)) AssertionError(false).throws(__trace(class)); }
#endif //CORE_ASSERT

#ifndef CORE_FEATURE
#define CORE_FEATURE(feature) CORE_ENABLE_ ## feature + 0 == 1
#endif //CORE_FEATURE

    }

    class Object;

    class Boolean;

    class Byte;

    class Short;

    class Integer;

    class Long;

    class Float;

    class Double;

    class String;

    class Character;

    class Complex;

    class Void;

    class Throwable;

    class Exception;

    class Error;

    template<class E>
    class Enum;


}

// primitives types
extern "C" {

CORE_WARNING_PUSH
CORE_WARNING_DISABLE_DEPRECATED
CORE_ALIAS(gbool, core::native::GENERIC_BOOL);
CORE_ALIAS(gbyte, core::native::GENERIC_INT8);
CORE_ALIAS(gshort, core::native::GENERIC_INT16);
CORE_ALIAS(gchar, core::native::GENERIC_CHR16);
CORE_ALIAS(gint, core::native::GENERIC_INT32);
CORE_ALIAS(glong, core::native::GENERIC_INT64);
CORE_ALIAS(gfloat, core::native::GENERIC_FLT32);
CORE_ALIAS(gdouble, core::native::GENERIC_FLT64);
CORE_WARNING_POP
CORE_STATIC_ASSERT(sizeof(gbool) == 1 && sizeof(gbyte) == 1 &&
                   sizeof(gshort) == 2 && sizeof(gchar) == 2 &&
                   sizeof(gint) == 4 && sizeof(gfloat) == 4 &&
                   sizeof(glong) == 8 && sizeof(gdouble) == 8,
                   "This compiler not supported. On architecture:" CORE_ARCH);

}


using core::native::GENERIC_PTR;

#if defined(CORE_COMPILER_MSVC) && !defined(__clang__)
struct _C_double_complex;
#endif


#endif //CORE23_GENERICS_H
