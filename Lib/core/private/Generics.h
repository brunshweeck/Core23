//
// Created by Brunshweeck on 11/09/2023 17:59.
// Copyright (c) 2023
//

#ifndef CORE23_GENERICS_H
#define CORE23_GENERICS_H

#include <core/private/Compiler.h>

namespace core {

    namespace io {}
    namespace charset {}
    namespace math {}
    namespace net {}
    namespace util {}
    namespace sql {}
    namespace text {}
    namespace time {}
    namespace function {}
    namespace concurrent {}
    namespace regex {}
    namespace random {}
    namespace gui {}
    namespace primitive {
        class BoolArray;
        class ByteArray;
        class ShortArray;
        class CharArray;
        class IntArray;
        class LongArray;
        class FloatArray;
        class DoubleArray;
    }

    namespace CORE_DEPRECATED native {


#ifndef CORE_ALIAS
#define CORE_ALIAS(alias, target, ...) using alias = target __VA_ARGS__
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
#define CORE_STATIC_CAST(class, expr) static_cast<class>(expr)
#define CORE_CONST_CAST(class, expr) const_cast<class>(expr)
#define CORE_PTR_CAST(class, expr) reinterpret_cast<class>(expr)
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

        namespace Generics {

            CORE_ALIAS(GENERIC_BOOLEAN, bool);
            CORE_ALIAS(GENERIC_BYTE_ALT, enum : signed char {
            });
            CORE_ALIAS(GENERIC_BYTE, signed char);
            CORE_ALIAS(GENERIC_FLOAT32, float);
            CORE_ALIAS(GENERIC_FLOAT64_ALT, long double);
            CORE_ALIAS(GENERIC_FLOAT64, double);
            CORE_ALIAS(GENERIC_CHR16, char16_t);
            CORE_ALIAS(GENERIC_INT16, signed short);
            CORE_ALIAS(GENERIC_INT32, signed int);
            CORE_ALIAS(GENERIC_INT64, signed long);
            CORE_ALIAS(GENERIC_INT64_ALT, signed long long);

        }


#define CORE_DISABLE_COPY(Class)            \
    Class(const Class &) = delete;          \
    Class &operator=(const Class &) = delete;

#define CORE_DISABLE_COPY_MOVE(Class) \
    CORE_DISABLE_COPY(Class)          \
    Class(Class &&) = delete;         \
    Class &operator=(Class &&) = delete;

#define CORE_ENABLE_IMPLICIT_CAST(class, op, ...)   operator class () __VA_ARGS__ { return CORE_CAST(class, op); }

#ifdef CORE_SHOW_TRACE_MODULE
#ifndef CORE_TRACE_MODULE
#define CORE_TRACE_MODULE "", "" // on format: [module-name], [module-version]
#endif
#define __trace(classname) Trace(CORE_GENERIC_MODULE CORE_GENERIC_VERSION classname, &CORE_FUNCTION[0], &CORE_FILE[0], CORE_LINE)
#define __ftrace(classname) Trace(CORE_GENERIC_MODULE CORE_GENERIC_VERSION classname, &CORE_FUNCTION_SIGNATURE[0], &CORE_FILE[0], CORE_LINE)
#else
// trace represent a simple function/method trace (trace with function name).
#define __trace(...) Trace(__VA_ARGS__, &CORE_FUNCTION[0], &CORE_FILE[0], CORE_LINE)

// ftrace represent a full function/method trace (trace with function signature).
#define __ftrace(...) Trace(__VA_ARGS__, &CORE_FUNCTION_SIGNATURE[0], &CORE_FILE[0], CORE_LINE)

#endif //CORE_SHOW_TRACE_MODULE

#ifdef CORE_XCOMPILER_UDL
#define CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, params, ...) extern const class operator"" ## suffix (params __VA_ARGS__)

#else
#define CORE_ENABLE_GENERIC_LITERAL(class, suffix, params,...)
#endif //CORE_XCOMPILER_UDL

#ifndef CORE_ASSERT
#define CORE_ASSERT(expr, class) if(!(expr)) AssertionError(false).throws(__trace(class)); else CORE_IGNORE(0)
#endif //CORE_ASSERT

#ifndef CORE_FEATURE
#define CORE_FEATURE(feature) CORE_ENABLE_ ## feature + 0 == 1
#endif //CORE_FEATURE

    }



    // basics class

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

    template<class Enumerable>
    class Enum;


}

CORE_WARNING_PUSH
CORE_WARNING_DISABLE_DEPRECATED

// primitives types
namespace {



    CORE_ALIAS(gbool, core::native::Generics::GENERIC_BOOLEAN);
    CORE_ALIAS(gshort, core::native::Generics::GENERIC_INT16);
    CORE_ALIAS(gchar, core::native::Generics::GENERIC_CHR16);
    CORE_ALIAS(gint, core::native::Generics::GENERIC_INT32);
    CORE_ALIAS(gfloat, core::native::Generics::GENERIC_FLOAT32);

    CORE_STATIC_ASSERT(
            sizeof(gbool) == 1 &&
            sizeof(gshort) == sizeof(gchar) &&
            sizeof(gint) == sizeof(gfloat) &&
            sizeof(gshort) == (sizeof(gbool) << 1) &&
            sizeof(gint) == (sizeof(gshort) << 1), "This compiler not supported. On architecture:" CORE_ARCH);

}

CORE_WARNING_POP

#include <exception>

namespace core {

    namespace native {
        CORE_ALIAS(GENERIC_THROWABLE, std::exception);
    }
}

#endif //CORE23_GENERICS_H
