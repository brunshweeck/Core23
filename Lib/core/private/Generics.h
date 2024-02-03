//
// Created by Brunshweeck on 11/09/2023 17:59.
// Copyright (c) 2023
//

#ifndef CORE23_GENERICS_H
#define CORE23_GENERICS_H

#include "Compiler.h"

namespace core {

    namespace foreign {
    }

    namespace native {}

    /**
     * Namespace for I/O Management
     */
    namespace io {

        using namespace native;

        class FileSystem;

        class File;

        class FileArray;

        class FileDescriptor;

        class IOException;

        class IOError;

        class FileReader;

        class FileWriter;

        class Reader;

        class Writer;

        class BufferedReader;

        class BufferedWriter;

        class ByteBuffer;

        class ShortBuffer;

        class CharBuffer;

        class IntBuffer;

        class LongBuffer;

        class FloatBuffer;

        class DoubleBuffer;
    }

    /**
     * Namespace of Encoding/Decoding String
     */
    namespace charset {
        using namespace native;

        class Charset;

        class CharsetEncoder;

        class CharsetDecoder;

        class CoderResult;
    }

    /**
     * Namespace of Internet Protocol Management
     */
    namespace net {
        using namespace native;

        class URI;

        class URL;

        class IPAddress;

        class IPv4Address;

        class IPv6Address;

        class Socket;
    }

    /**
     * Namespace used for Databases Management
     */
    namespace sql {}

    /**
     * Namespace for text formatting
     */
    namespace text {
        class Collator;
    }

    /**
     * Namespace of Temporal structure, Timer, and chronology
     */
    namespace time {
        using namespace native;

        class LocalDate;

        class LocalTime;

        class LocalDateTime;

        class ZoneRegion;

        class ZoneOffset;

        class ZoneRules;

        class ZoneOffsetTransition;

        class ZoneOffsetTransitionRule;

        class Calendar;

        class Era;
    }

    namespace function {

    }

    namespace stream {
        using namespace function;

        template<class>
        class Stream;

        class DoubleStream;

        class IntStream;

        class LongStream;
    }

    /**
     * Namespace for utility and algorithms
     */
    namespace util {

        using namespace native;
        using namespace function;
        using namespace stream;

        template<class>
        class Collection;

        template<class>
        class List;

        template<class>
        class Set;

        template<class>
        class Queue;

        template<class>
        class ArrayList;

        template<class>
        class LinkedList;

        template<class>
        class Vector;

        template<class>
        class HashSet;

        template<class>
        class LinkedHashSet;

        template<class>
        class TreeSet;

        template<class>
        class PriorityQueue;

        template<class, class>
        class Map;

        template<class, class>
        class HashMap;

        template<class, class>
        class LinkedHashMap;

        template<class, class>
        class HashTable;

        template<class, class>
        class TreeMap;

        template<class>
        class Comparator;

        template<class>
        class Iterator;

        template<class>
        class ListIterator;

        class Locale;

        class Thread;

        class StringTokenizer;

        template<class>
        class Optional;
        class OptionalInt;
        class OptionalLong;
        class OptionalDouble;

        using namespace native;
    }

    namespace function {

        template<class>
        class Consumer;

        template<class>
        class Predicate;

        template<class, class>
        class Function;

        template<class>
        class Supplier;

        template<class, class>
        class BiConsumer;

        template<class, class>
        class BiPredicate;

        template<class, class, class>
        class BiFunction;

        template<class>
        class UnaryOperator;

        template<class>
        class BinaryOperator;

        class DoubleConsumer;

        class DoublePredicate;

        class DoubleSupplier;

        class DoubleUnaryOperator;

        class DoubleBinaryOperator;

        template<class>
        class DoubleFunction;

        template<class>
        class ToDoubleFunction;

        class DoubleToIntFunction;

        class DoubleToLongFunction;

        class LongConsumer;

        class LongPredicate;

        class LongSupplier;

        class LongUnaryOperator;

        class LongBinaryOperator;

        template<class>
        class LongFunction;

        template<class>
        class ToLongFunction;

        class LongToIntFunction;

        class LongToDoubleFunction;

        class IntConsumer;

        class IntPredicate;

        class IntSupplier;

        class IntUnaryOperator;

        class IntBinaryOperator;

        template<class>
        class IntFunction;

        template<class>
        class ToIntFunction;

        class IntToLongFunction;

        class IntToDoubleFunction;
    }

    namespace stream {
        using namespace util;
    }

    namespace concurrent {

        using namespace native;
        using namespace util;

        class AtomicBoolean;

        class AtomicInteger;

        class AtomicLong;

        class AtomicIntegerArray;

        class AtomicLongArray;

        template<class>
        class AtomicReference;

        template<class>
        class AtomicReferenceArray;

        class Lock;

        class Condition;
    }

    namespace regex {

        using namespace native;
        using namespace util;

        class Pattern; // regular expression
        class Matcher;
    }

    namespace random {

        using namespace native;
        using namespace util;
        using namespace stream;

        class Random;
    }

    namespace gui {

        using namespace native;
        using namespace util;
    }

    namespace native {

        /**
         * Base of all primitive array
         */
        template<class>
        class PrimitiveArray; // T[x]

        /**
         * Base of all two dimensional primitive array
         */
        template<class>
        class PrimitiveArray2D; // T[x][y]

        /**
         * Base of all reference array
         */
        class ObjectArray; // Object[x]

        /**
         * The reference array
         */
        template<class>
        class Array; // T[x]

        class BooleanArray; // gbool[x]
        class ByteArray; // gbyte[x]
        class ShortArray; // gshort[x]
        class CharArray; // gchar[x]
        class IntArray; // gint[x]
        class LongArray; // glong[x]
        class FloatArray; // gfloat[x]
        class DoubleArray; // gdouble[x]
        class StringArray; // String[x]

        class BooleanArray2D; // gbool[x][y]
        class ByteArray2D; // gbyte[x][y]
        class ShortArray2D; // gshort[x][y]
        class CharArray2D; // gchar[x][y]
        class IntArray2D; // gint[x][y]
        class LongArray2D; // glong[x][y]
        class FloatArray2D; // gfloat[x][y]
        class DoubleArray2D; // gdouble[x][y]
        class StringArray2D; // String[x][y]

        class CORE_XDEPRECATED("Use Array<Object> instead") ReferenceArray;

        class Unsafe;
    }

    class Object;

    class Boolean; // gbool
    class Byte; // gbyte
    class Short; // gshort
    class Character; // gchar
    class Integer; // gint
    class Long; // glong
    class Float; // gfloat
    class Double; // gdouble
    class Void; // void
    class Complex; // __Complex, __C_Complex
    template<class>
    class Enum; // enum T, enum class T
    class String; // CharT[n], CharT*
    class Throwable; // std::exception
    class Exception; // std::exception
    class Error; // std::exception

    class Runnable;


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
    using native::Array;
    using native::Unsafe;

    namespace native {


#ifndef CORE_ALIAS
#define CORE_ALIAS(alias, target, ...) using alias = target __VA_ARGS__
#define CORE_USING(namespace, Element, ...) using namespace::target __VA_ARGS__
#endif //CORE_ALIAS

#ifndef CORE_FRATERNITY
#define CORE_FRATERNITY(T) friend T
#define CORE_FRATERNITY_T1(T) template <class> CORE_FRATERNITY(class) T
#define CORE_FRATERNITY_T2(T) template <class, class> CORE_FRATERNITY(class) T
#define CORE_FRATERNITY_Tx(T) template <class...> CORE_FRATERNITY(class) T
#endif // CORE_FRATERNITY

#ifndef CORE_STATIC_ASSERT
#define CORE_STATIC_ASSERT(expression, message) static_assert((expression), message)
#endif //CORE_STATIC_ASSERT

#ifndef CORE_FAST
#define CORE_FAST CORE_CONSTEXPR
#endif //CORE_FAST

#ifndef interface
#define interface struct
#endif //interface


#ifndef CORE_CAST
#define CORE_CAST(class, expression) ((class)(expression))
#define CORE_DYN_CAST(class, expression) (dynamic_cast<class>(expression))
#define CORE_CST_CAST(class, expression) (const_cast<class>(expression))
#endif //CORE_CAST

#ifndef CORE_IGNORE
#define CORE_IGNORE(expression) CORE_CAST(void, expression)
#endif //CORE_IGNORE

#ifndef CORE_HIDE
/**
 * This expression is used execute hidden expression and return the
 * visible expression
 *
 * @param visible the value returned
 * @param the value executed in background
 */
#define CORE_HIDE(visible, hidden, ...) ((hidden __VA_ARGS__), visible)
#endif

#ifndef CORE_FILE

#ifndef __FILE_NAME__
#define CORE_FILE __FILE__

#else
#define CORE_FILE __FILE__
#endif //__FILE_NAME__

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

#define CORE_ENABLE_IMPLICIT_CAST(class, Operator, MethodQualifiers)   \
    operator class () MethodQualifiers {                               \
        return CORE_CAST(class, Operator);                             \
    }

#ifdef CORE_SHOW_TRACE_MODULE
#ifndef CORE_TRACE_MODULE
#define CORE_TRACE_MODULE ""_S, ""_S, // on format: [module-name], [module-version],
#endif
#define __trace(className) Trace(CORE_TRACE_MODULE className, CORE_FUNCTION, CORE_FILE, CORE_LINE)
#define __ftrace(className) Trace(CORE_TRACE_MODULE className, CORE_FUNCTION_SIGNATURE, CORE_FILE, CORE_LINE)

#else
// The Current Execution point (with slimmed function signature)
#define __trace(className, ...) Trace(className __VA_ARGS__, CORE_FUNCTION, CORE_FILE, CORE_LINE)

// The Current Execution point (will full function signature)
#define __ftrace(className, ...) Trace(className __VA_ARGS__, CORE_FUNCTION_SIGNATURE, CORE_FILE, CORE_LINE)

#endif //CORE_SHOW_TRACE_MODULE

// Execution point used into global function
#define __xtrace() __trace(""_S)
// Execution point used into global function
#define __xftrace() __ftrace(""_S)

// Execution point used into the class method
#define __ctrace() __trace((*this).classname())
// Execution point used into the class method
#define __cftrace() __ftrace((*this).classname())

#ifdef CORE_XCOMPILER_UDL
#define CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, params, ...) extern const class operator"" ## suffix (params __VA_ARGS__)

#else
#define CORE_ENABLE_LITERAL_CONSTRUCTOR(class, suffix, params,...)
#endif //CORE_XCOMPILER_UDL

#ifndef CORE_ASSERT
/**
 * Macro used to defined assertion
 *
 * @param expression The boolean value evaluated during assertion
 * @param class The name or INSTANCE of class containing assertion
 */
#define CORE_ASSERT(expression, ...) CORE_RAISE(expression, u ## #expression ## _S, __VA_ARGS__)

#define CORE_RAISE(expression, message, ...) {if(!(expression)) AssertionError(message).throws(__VA_ARGS__);}

#endif //CORE_ASSERT

    }

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

#ifndef CORE_TRY_ONLY

#define CORE_TRY_ONLY(action, ...) try { action; } catch(const Throwable& th) { __VA_ARGS__; }

#define CORE_TRY_ONLY_EXCEPTION(action, ...) try { action; } catch(const Exception& ex) { __VA_ARGS__; }

#define CORE_TRY_ONLY_ERROR(action, ...) try { action; } catch(const Error& err) { __VA_ARGS__; }

#define CORE_TRY_ONLY_ANY(action, ...) try { action; } catch(...) { __VA_ARGS__; }

#define CORE_TRY_RETHROW(action, fallbackAction, newTrace) CORE_TRY_ONLY(action, fallbackAction; th.throws(newTrace))

#define CORE_TRY_RETHROW_ANY(action, fallbackAction) CORE_TRY_ONLY_ANY(action, fallbackAction; throw)

#define CORE_TRY_RETHROW_EXCEPTION(action, fallbackAction, newTrace) CORE_TRY_ONLY_EXCEPTION(action, fallbackAction; ex.throws(newTrace))

#define CORE_TRY_RETHROW_ERROR(action, fallbackAction, newTrace) CORE_TRY_ONLY_ERROR(action, fallbackAction; err.throws(newTrace))

#endif

#pragma push_macro("__trace")
#pragma push_macro("__ctrace")
#pragma push_macro("__xtrace")
#pragma push_macro("__ftrace")
#pragma push_macro("__xftrace")
#pragma push_macro("__cftrace")
#pragma push_macro("interface")

#pragma push_macro("CORE_TRY_ONLY")
#pragma push_macro("CORE_TRY_ONLY_EXCEPTION")
#pragma push_macro("CORE_TRY_ONLY_ERROR")
#pragma push_macro("CORE_TRY_ONLY_ANY")
#pragma push_macro("CORE_TRY_RETHROW")
#pragma push_macro("CORE_TRY_RETHROW_EXCEPTION")
#pragma push_macro("CORE_TRY_RETHROW_ERROR")
#pragma push_macro("CORE_TRY_RETHROW_ANY")


#endif //CORE23_GENERICS_H
