//
// Created by Brunshweeck on 12/09/2023.
//

#include "Unsafe.h"
#include <core/IllegalArgumentException.h>
#include <core/private/Null.h>
#include <typeinfo>

#if defined(__GNUC__)
#define USE_INTERLOCKED_FUNCTION 0

#elif defined(CORE_COMPILER_MSVC) || __has_include(<intrin.h>)

#include <intrin.h>


// use compiler intrinsics for all atomic functions
# define CORE_INTERLOCKED_PREFIX _
# define CORE_INTERLOCKED_PROTOTYPE
# define CORE_INTERLOCKED_DECLARE_PROTOTYPES
# define CORE_INTERLOCKED_INTRINSIC
# define CORE_ATOMIC_INT16_IS_SUPPORTED

# ifdef _WIN64
#  define CORE_ATOMIC_INT64_IS_SUPPORTED
# endif

////////////////////////////////////////////////////////////////////////////////////////////////////
// Prototype declaration

#define CORE_INTERLOCKED_CONCAT_I(prefix, suffix) \
    prefix ## suffix
#define CORE_INTERLOCKED_CONCAT(prefix, suffix) \
    CORE_INTERLOCKED_CONCAT_I(prefix, suffix)

// MSVC intrinsics prefix function names with an underscore. Also, if platform
// SDK headers have been included, the Interlocked names may be defined as
// macros.
// To avoid double underscores, we paste the prefix with Interlocked first and
// then the remainder of the function name.
#define CORE_INTERLOCKED_FUNCTION(name) \
    CORE_INTERLOCKED_CONCAT(CORE_INTERLOCKED_CONCAT(CORE_INTERLOCKED_PREFIX, Interlocked), name)

#ifndef CORE_INTERLOCKED_VOLATILE
# define CORE_INTERLOCKED_VOLATILE volatile
#endif

#ifndef CORE_INTERLOCKED_PREFIX
#define CORE_INTERLOCKED_PREFIX
#endif

#ifndef CORE_INTERLOCKED_PROTOTYPE
#define CORE_INTERLOCKED_PROTOTYPE
#endif

#ifdef CORE_INTERLOCKED_DECLARE_PROTOTYPES
#undef CORE_INTERLOCKED_DECLARE_PROTOTYPES

extern "C" {

long CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Increment)(long CORE_INTERLOCKED_VOLATILE *);
long CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Decrement)(long CORE_INTERLOCKED_VOLATILE *);
long CORE_INTERLOCKED_PROTOTYPE
CORE_INTERLOCKED_FUNCTION(CompareExchange)(long CORE_INTERLOCKED_VOLATILE *, long, long);
long CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Exchange)(long CORE_INTERLOCKED_VOLATILE *, long);
long CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(ExchangeAdd)(long CORE_INTERLOCKED_VOLATILE *, long);
typedef long CORE_INTERLOCKED_VOLATILE *XADDRESS;
typedef long CORE_INTERLOCKED_VOLATILE XVALUE;

# if !defined(__i386__) && !defined(_M_IX86)
void *CORE_INTERLOCKED_FUNCTION(CompareExchangePointer)(void *CORE_INTERLOCKED_VOLATILE *, void *, void *);
void *CORE_INTERLOCKED_FUNCTION(ExchangePointer)(void *CORE_INTERLOCKED_VOLATILE *, void *);
__int64 CORE_INTERLOCKED_FUNCTION(ExchangeAdd64)(__int64 CORE_INTERLOCKED_VOLATILE *, __int64);
# endif

# ifdef CORE_ATOMIC_INT16_IS_SUPPORTED
short CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Increment16)(short CORE_INTERLOCKED_VOLATILE *);
short CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Decrement16)(short CORE_INTERLOCKED_VOLATILE *);
short CORE_INTERLOCKED_PROTOTYPE
CORE_INTERLOCKED_FUNCTION(CompareExchange16)(short CORE_INTERLOCKED_VOLATILE *, short, short);
short CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Exchange16)(short CORE_INTERLOCKED_VOLATILE *, short);
short CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(ExchangeAdd16)(short CORE_INTERLOCKED_VOLATILE *, short);

# endif
# ifdef CORE_ATOMIC_INT64_IS_SUPPORTED
__int64 CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Increment64)(__int64 CORE_INTERLOCKED_VOLATILE *);
__int64 CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Decrement64)(__int64 CORE_INTERLOCKED_VOLATILE *);
__int64 CORE_INTERLOCKED_PROTOTYPE
CORE_INTERLOCKED_FUNCTION(CompareExchange64)(__int64 CORE_INTERLOCKED_VOLATILE *, __int64, __int64);
__int64 CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION(Exchange64)(__int64 CORE_INTERLOCKED_VOLATILE *, __int64);
//above already: glong CORE_INTERLOCKED_PROTOTYPE CORE_INTERLOCKED_FUNCTION( ExchangeAdd64 )(glong CORE_INTERLOCKED_VOLATILE *, glong);
# endif
}

#endif // CORE_INTERLOCKED_DECLARE_PROTOTYPES

#undef CORE_INTERLOCKED_PROTOTYPE

////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CORE_INTERLOCKED_INTRINSIC
#undef CORE_INTERLOCKED_INTRINSIC

# pragma intrinsic (_InterlockedIncrement)
# pragma intrinsic (_InterlockedDecrement)
# pragma intrinsic (_InterlockedExchange)
# pragma intrinsic (_InterlockedCompareExchange)
# pragma intrinsic (_InterlockedExchangeAdd)

# if !defined(_M_IX86)
#pragma intrinsic (_InterlockedCompareExchangePointer)
#pragma intrinsic (_InterlockedExchangePointer)
#pragma intrinsic (_InterlockedExchangeAdd64)
# endif

#endif // CORE_INTERLOCKED_INTRINSIC

////////////////////////////////////////////////////////////////////////////////////////////////////
// Interlocked* replacement macros

#if defined(__i386__) || defined(_M_IX86)

# define CORE_INTERLOCKED_COMPARE_EXCHANGE_POINTER(value, newValue, expectedValue) \
    (void*)(CORE_INTERLOCKED_FUNCTION(CompareExchange)((long CORE_INTERLOCKED_VOLATILE *)(value), long(newValue), long(expectedValue)))

# define CORE_INTERLOCKED_EXCHANGE_POINTER(value, newValue) \
    CORE_INTERLOCKED_FUNCTION(Exchange)((long CORE_INTERLOCKED_VOLATILE *)(value), long(newValue))

# define CORE_INTERLOCKED_EXCHANGE_ADD_POINTER(value, valueToAdd) \
    CORE_INTERLOCKED_FUNCTION(ExchangeAdd)( (long CORE_INTERLOCKED_VOLATILE *)(value), (valueToAdd))

#else // !defined(__i386__) && !defined(_M_IX86)

# define CORE_INTERLOCKED_COMPARE_EXCHANGE_POINTER(value, newValue, expectedValue) \
    CORE_INTERLOCKED_FUNCTION(CompareExchangePointer)((void * CORE_INTERLOCKED_VOLATILE *)(value), (void *) (newValue), (void *) (expectedValue))

# define CORE_INTERLOCKED_EXCHANGE_POINTER(value, newValue) \
    CORE_INTERLOCKED_FUNCTION(ExchangePointer)((void * CORE_INTERLOCKED_VOLATILE *)(value), (void *) (newValue))

# define CORE_INTERLOCKED_EXCHANGE_ADD_POINTER(value, valueToAdd) \
    CORE_INTERLOCKED_FUNCTION(ExchangeAdd64)((glong CORE_INTERLOCKED_VOLATILE *)(value), (valueToAdd))

#endif // !defined(__i386__) && !defined(_M_IX86)


#define USE_INTERLOCKED_FUNCTION 1

#endif


#ifdef CORE_COMPILER_GNU_ONLY
// gcc, mingw, clang (unix)

#if CORE_COMPILER_GNU > 405
#define CORE_EXPR(ex_gnu, gnu, interLocker, def) gnu

#else
#define CORE_EXPR(ex_gnu, gnu, interLocker, def) ex_gnu

#endif

#else

#if __has_builtin(__atomic_load_n)
#define CORE_EXPR(ex_gnu, gnu, interLocker, def) gnu

#elif __has_builtin(__sync_fetch_and_add)
#define CORE_EXPR(ex_gnu, gnu, interLocker, def) ex_gnu

#elif USEUSE_INTERLOCKED_FUNCTION
#define CORE_EXPR(ex_gnu, gnu, interLocker, def) interLocker

#else
#define CORE_EXPR(ex_gnu, gnu, interLocker, def) def

#endif

#endif

#ifdef CORE_COMPILER_MSVC
#define CORE_BARRIER(expr) \
    CORE_WARNING_PUSH      \
    CORE_WARNING_DISABLE_MSVC(4996) \
    _ReadWriteBarrier();   \
    CORE_WARNING_POP       \
    expr
#else
#define CORE_BARRIER(expr) \
    expr
#endif

namespace core {
    CORE_ALIAS(U, native::Unsafe);

    namespace native {

        gint Unsafe::getInt(const Object &o, glong offset) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            return *(gint *) address;
        }

        void Unsafe::putInt(Object &o, glong offset, gint x) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            *(gint *) address = x;
        }

        gint Unsafe::getInt(glong address) {
            try {
                return getInt(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putInt(glong address, gint x) {
            try {
                putInt(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gbool Unsafe::getBoolean(const Object &o, glong offset) {
            try {
                return b2bool(getByte(o, offset));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putBoolean(Object &o, glong offset, gbool x) {
            try {
                putByte(o, offset, b2byte(x));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gbool Unsafe::getBoolean(glong address) {
            try {
                return getBoolean(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putBoolean(glong address, gbool x) {
            try {
                putBoolean(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gbyte Unsafe::getByte(const Object &o, glong offset) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            return *(gbyte *) address;
        }

        void Unsafe::putByte(Object &o, glong offset, gbyte x) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            *(gbyte *) address = x;
        }

        gbyte Unsafe::getByte(glong address) {
            try {
                return getByte(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putByte(glong address, gbyte x) {
            try {
                putByte(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gshort Unsafe::getShort(const Object &o, glong offset) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            return *(gshort *) address;
        }

        void Unsafe::putShort(Object &o, glong offset, gshort x) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            *(gshort *) address = x;
        }

        gshort Unsafe::getShort(glong address) {
            try {
                return getShort(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putShort(glong address, gshort x) {
            try {
                putShort(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gchar Unsafe::getChar(const Object &o, glong offset) {
            try {
                return s2c(getShort(o, offset));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putChar(Object &o, glong offset, gchar x) {
            try {
                putShort(o, offset, c2s(x));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gchar Unsafe::getChar(glong address) {
            try {
                return getChar(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putChar(glong address, gchar x) {
            try {
                putChar(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        glong Unsafe::getLong(const Object &o, glong offset) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            return *(glong *) address;
        }

        void Unsafe::putLong(Object &o, glong offset, glong x) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong const address = getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to null address").throws(__trace("core.private.Unsafe"));
            *(glong *) address = x;
        }

        glong Unsafe::getLong(glong address) {
            try {
                return getLong(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putLong(glong address, glong x) {
            try {
                putLong(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gfloat Unsafe::getFloat(const Object &o, glong offset) {
            try {
                return i2f(getInt(o, offset));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putFloat(Object &o, glong offset, gfloat x) {
            try {
                putInt(o, offset, f2i(x));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gfloat Unsafe::getFloat(glong address) {
            try {
                return getFloat(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putFloat(glong address, gfloat x) {
            try {
                putFloat(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gdouble Unsafe::getDouble(const Object &o, glong offset) {
            try {
                return l2d(getLong(o, offset));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putDouble(Object &o, glong offset, gdouble x) {
            try {
                putLong(o, offset, d2l(x));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        gdouble Unsafe::getDouble(glong address) {
            try {
                return getDouble(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putDouble(glong address, gdouble x) {
            try {
                putDouble(null, address, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        Object &Unsafe::getReference(const Object &o, glong offset) {
            try {
                return l2o(getAddress(o, offset));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putReference(Object &o, glong offset, Object &x) {
            try {
                putAddress(o, offset, o2l(x));
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        glong Unsafe::getAddress(const Object &o, glong offset) {
            if (!checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            try {
                return ADDRESS_SIZE == 4 ? (glong) getInt(o, offset) : getLong(o, offset);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putAddress(Object &o, glong offset, glong x) {
            if (!checkPointer(o, offset) || !checkNativeAddress(x))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            try {
                if (ADDRESS_SIZE == 4)
                    putInt(o, offset, (gint) x);
                else
                    putLong(o, offset, x);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        glong Unsafe::getAddress(glong address) {
            try {
                return getAddress(null, address);
            } catch (const Throwable &thr) {
                thr.throws(__trace("core.private.Unsafe"));
            }
        }

        void Unsafe::putAddress(glong address, glong x) { putAddress(null, address, x); }

        glong Unsafe::allocateMemory(glong sizeInBytes) {
            glong const heapSize = alignToHeapWordSize(sizeInBytes);
            if (!checkSize(heapSize))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            if (heapSize == 0) return 0;
            glong const address = allocateMemoryImpl(heapSize);
            if (address == 0)
                MemoryError("Unable to allocate " + String::valueOf(heapSize) + " bytes")
                        .throws(__trace("core.private.Unsafe"));
            return address;
        }

        glong Unsafe::reallocateMemory(glong address, glong sizeInBytes) {
            glong const heapSize = alignToHeapWordSize(sizeInBytes);
            if (!checkSize(heapSize) || !checkPointer(null, address))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            if (heapSize == 0) {
                freeMemory(address);
                return 0;
            }
            glong const address2 =
                    address == 0 ? allocateMemoryImpl(heapSize) : reallocateMemoryImpl(address, heapSize);
            if (address2 == 0)
                MemoryError("Unable to allocate " + String::valueOf(heapSize) + " bytes")
                        .throws(__trace("core.private.Unsafe"));
            return address2;
        }

        void Unsafe::setMemory(Object &o, glong offset, glong sizeInBytes, gbyte value) {
            if (!checkSize(sizeInBytes) || !checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            if (sizeInBytes > 0)
                setMemoryImpl(getNativeAddress(o, offset), sizeInBytes, value);
        }

        void Unsafe::setMemory(glong address, glong sizeInBytes, gbyte value) {
            setMemory(null, address, sizeInBytes, value);
        }

        void Unsafe::copyMemory(const Object &src, glong srcOffset, Object &dest, glong destOffset, glong sizeInBytes) {
            if (!checkSize(sizeInBytes) || !checkPointer(src, srcOffset) || !checkPointer(dest, destOffset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            if (sizeInBytes == 0) return;
            copyMemoryImpl((glong) &src + srcOffset, (glong) &dest + destOffset, sizeInBytes);
        }

        void Unsafe::copyMemory(glong srcAddress, glong destAddress, glong sizeInBytes) {
            copyMemory(null, srcAddress, null, destAddress, sizeInBytes);
        }

        void Unsafe::copySwapMemory(const Object &src, glong srcOffset,
                                    Object &dest, glong destOffset,
                                    glong sizeInBytes, glong elemSize) {
            if (!checkSize(sizeInBytes) || !checkPointer(src, srcOffset) || !checkPointer(dest, destOffset) ||
                (elemSize != 2 && elemSize != 4 && elemSize != 8 && sizeInBytes % elemSize != 0))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));

            if (sizeInBytes > 0)
                copySwapMemoryImpl(getNativeAddress(src, srcOffset),
                                   getNativeAddress(dest, destOffset), sizeInBytes, elemSize);
        }

        void Unsafe::copySwapMemory(glong srcAddress, glong destAddress, glong sizeInBytes, glong elemSize) {
            copySwapMemory(null, srcAddress, null, destAddress, sizeInBytes, elemSize);
        }

        void Unsafe::freeMemory(glong address) {
            if (!checkPointer(null, address))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            if (address != 0) {
                freeMemoryImpl(address);
            }

        }

        namespace {
            interface Heap {
                glong address = 0;
                Heap *next = null;
                Heap *prev = null;

                CORE_FAST Heap(glong address, Heap *prev, Heap *next) :
                        address(address), prev(prev), next(next) {}
            };

            class Cache CORE_FINAL : public Object {
            private:
                Heap *head = {};
                Heap *tail = {};
                gint count = 0;
                enum CacheState : gbool {
                    UNLOCKED,
                    LOCKED,
                } state = UNLOCKED;

            public:
                CORE_FAST Cache() = default;

                CORE_FAST Cache(const Cache &) = default;

                CORE_FAST Cache(Cache &&) CORE_NOTHROW = default;

                void add(glong address) {
                    if (state == LOCKED) {
                        // wait
                    } else if (count == Integer::MAX_VALUE) {
                        return;
                    }
                    state = LOCKED;
                    try {
                        head = new Heap(address, null, head);
                        (head->next != null ? head->next->prev : tail) = head;
                        count += 1;
                    } catch (const GENERIC_THROWABLE &) {}
                    state = UNLOCKED;
                }

                gbool exists(glong address) {
                    if (state == LOCKED) {
                        // wait
                    }
                    state = LOCKED;
                    Heap *first = head;
                    Heap *last = tail;
                    gint i = 0;
                    while (i < count) {
                        if (first->address == address) {
                            state = UNLOCKED;
                            return true;
                        }
                        if (last->address == address) {
                            state = UNLOCKED;
                            return true;
                        }
                        i += 2;
                        first = first->next;
                        last = last->prev;
                    }
                    state = UNLOCKED;
                    return false;
                }

                void remove(glong address) {
                    if (state == LOCKED) {
                        // wait
                    }
                    state = LOCKED;
                    Heap *first = head;
                    Heap *last = tail;
                    gint i = 0;
                    while (i < count) {
                        if (first->address == address) {
                            Heap *p = first->prev;
                            Heap *n = first->next;
                            if (first == head)
                                head = n;
                            if (first == tail)
                                tail = p;
                            if (p != null)
                                p->next = n;
                            if (n != null)
                                n->prev = p;
                            first->prev = first->next = null;
                            delete first;
                            state = UNLOCKED;
                            return;
                        }
                        if (last->address == address) {
                            Heap *p = last->prev;
                            Heap *n = last->next;
                            if (last == head)
                                head = n;
                            if (last == tail)
                                tail = p;
                            if (p != null)
                                p->next = n;
                            if (n != null)
                                n->prev = p;
                            last->prev = last->next = null;
                            delete last;
                            state = UNLOCKED;
                            return;
                        }
                        i += 2;
                        first = first->next;
                        last = last->prev;
                    }
                    state = UNLOCKED;
                }

                ~Cache() override {
                    if (state == LOCKED) {
                        // wait
                    }
                    state = LOCKED;
                    // clean all memory caches
                    while (head != null) {
                        Heap *heap = head;
                        head = heap->next;
                        if (head != null)
                            head->prev = null;
                        Unsafe::freeMemory(heap->address);
                        heap->next = heap->prev = null;
                        heap->address = 0;
                        delete heap;
                        count -= 1;
                    }
                    tail = head = null;
                    state = UNLOCKED;
                }
            };

            Cache cache = {};
        }

        gbool Unsafe::loadInstance(glong address) { return cache.exists(address); }

        void Unsafe::storeInstance(glong address) { cache.add(address); }

        void Unsafe::deleteInstance(glong address) { cache.remove(address); }

        glong Unsafe::getLongUnaligned(const Object &o, glong offset) {
            if ((offset & 7) == 0) {
                return getLong(o, offset);
            } else if ((offset & 3) == 0) {
                return makeLong(getInt(o, offset),
                                getInt(o, offset + 4));
            } else if ((offset & 1) == 0) {
                return makeLong(getShort(o, offset),
                                getShort(o, offset + 2),
                                getShort(o, offset + 4),
                                getShort(o, offset + 6));
            } else {
                return makeLong(getByte(o, offset),
                                getByte(o, offset + 1),
                                getByte(o, offset + 2),
                                getByte(o, offset + 3),
                                getByte(o, offset + 4),
                                getByte(o, offset + 5),
                                getByte(o, offset + 6),
                                getByte(o, offset + 7));
            }
        }

        glong Unsafe::getLongUnaligned(const Object &o, glong offset, gbool bigEndian) {
            return convEndian(bigEndian, getLongUnaligned(o, offset));
        }

        gint Unsafe::getIntUnaligned(const Object &o, glong offset) {
            if ((offset & 3) == 0) {
                return getInt(o, offset);
            } else if ((offset & 1) == 0) {
                return makeInt(getShort(o, offset),
                               getShort(o, offset + 2));
            } else {
                return makeInt(getByte(o, offset),
                               getByte(o, offset + 1),
                               getByte(o, offset + 2),
                               getByte(o, offset + 3));
            }
        }

        gint Unsafe::getIntUnaligned(const Object &o, glong offset, gbool bigEndian) {
            return convEndian(bigEndian, getIntUnaligned(o, offset));
        }

        gshort Unsafe::getShortUnaligned(const Object &o, glong offset) {
            if ((offset & 1) == 0) {
                return getShort(o, offset);
            } else {
                return makeShort(getByte(o, offset),
                                 getByte(o, offset + 1));
            }
        }

        gshort Unsafe::getShortUnaligned(const Object &o, glong offset, gbool bigEndian) {
            return convEndian(bigEndian, getShortUnaligned(o, offset));
        }

        gchar Unsafe::getCharUnaligned(const Object &o, glong offset) {
            if ((offset & 1) == 0) {
                return getChar(o, offset);
            } else {
                return (gchar) makeShort(getByte(o, offset),
                                         getByte(o, offset + 1));
            }
        }

        gchar Unsafe::getCharUnaligned(const Object &o, glong offset, gbool bigEndian) {
            return convEndian(bigEndian, getCharUnaligned(o, offset));
        }

        void Unsafe::putLongUnaligned(Object &o, glong offset, glong x) {
            if ((offset & 7) == 0) {
                putLong(o, offset, x);
            } else if ((offset & 3) == 0) {
                putLongParts(o, offset,
                             (gint) (x >> 0),
                             (gint) (x >> 32));
            } else if ((offset & 1) == 0) {
                putLongParts(o, offset,
                             (gshort) (x >> 0),
                             (gshort) (x >> 16),
                             (gshort) (x >> 32),
                             (gshort) (x >> 48));
            } else {
                putLongParts(o, offset,
                             (gbyte) (x >> 0),
                             (gbyte) (x >> 8),
                             (gbyte) (x >> 16),
                             (gbyte) (x >> 24),
                             (gbyte) (x >> 32),
                             (gbyte) (x >> 40),
                             (gbyte) (x >> 48),
                             (gbyte) (x >> 56));
            }
        }

        void Unsafe::putLongUnaligned(Object &o, glong offset, glong x, gbool bigEndian) {
            putLongUnaligned(o, offset, convEndian(bigEndian, x));
        }

        void Unsafe::putIntUnaligned(Object &o, glong offset, gint x) {
            if ((offset & 3) == 0) {
                putInt(o, offset, x);
            } else if ((offset & 1) == 0) {
                putIntParts(o, offset,
                            (gshort) (x >> 0),
                            (gshort) (x >> 16));
            } else {
                putIntParts(o, offset,
                            (gbyte) (x >> 0),
                            (gbyte) (x >> 8),
                            (gbyte) (x >> 16),
                            (gbyte) (x >> 24));
            }
        }

        void Unsafe::putIntUnaligned(Object &o, glong offset, gint x, gbool bigEndian) {
            putIntUnaligned(o, offset, convEndian(bigEndian, x));
        }

        void Unsafe::putShortUnaligned(Object &o, glong offset, gshort x) {
            if ((offset & 1) == 0) {
                putShort(o, offset, x);
            } else {
                putShortParts(o, offset,
                              (gbyte) (x >> 0),
                              (gbyte) (x >> 8));
            }
        }

        void Unsafe::putShortUnaligned(Object &o, glong offset, gshort x, gbool bigEndian) {
            putShortUnaligned(o, offset, convEndian(bigEndian, x));
        }

        void Unsafe::putCharUnaligned(Object &o, glong offset, gchar x) {
            putShortUnaligned(o, offset, c2s(x));
        }

        void Unsafe::putCharUnaligned(Object &o, glong offset, gchar x, gbool bigEndian) {
            putCharUnaligned(o, offset, convEndian(bigEndian, x));
        }

        gbool Unsafe::compareAndSetReference(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &compareAndExchangeReference(o, offset, expected, x) == &x;
        }

        gbool Unsafe::compareAndSetReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &compareAndExchangeReferenceAcquire(o, offset, expected, x) == &x;
        }

        gbool Unsafe::compareAndSetReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &compareAndExchangeReferenceRelaxed(o, offset, expected, x) == &x;
        }

        gbool Unsafe::compareAndSetReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &compareAndExchangeReferenceRelease(o, offset, expected, x) == &x;
        }

        gbool Unsafe::weakCompareAndSetReference(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &weakCompareAndExchangeReference(o, offset, expected, x) == &x;
        }

        gbool Unsafe::weakCompareAndSetReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &weakCompareAndExchangeReferenceAcquire(o, offset, expected, x) == &x;
        }

        gbool Unsafe::weakCompareAndSetReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &weakCompareAndExchangeReferenceRelaxed(o, offset, expected, x) == &x;
        }

        gbool Unsafe::weakCompareAndSetReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return &weakCompareAndExchangeReferenceRelease(o, offset, expected, x) == &x;
        }

        gbool Unsafe::compareAndSetBoolean(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeBoolean(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeBooleanAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeBooleanRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeBooleanRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBoolean(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeBoolean(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeBooleanAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeBooleanRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeBooleanRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeByte(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeByteAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeByteRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeByteRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeByte(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeByteAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeByteRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeByteRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShort(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeShort(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeShortAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeShortRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShortRelaxed(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeShortRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShort(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeShort(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeShortAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeShortRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShortRelaxed(Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeShortRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetChar(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeChar(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeCharAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeCharRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeCharRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetChar(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeChar(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeCharAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeCharRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeCharRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetInt(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeInt(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeIntAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetIntRelease(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeIntRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetIntRelaxed(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeIntRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetInt(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeInt(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeIntAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetIntRelease(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeIntRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetIntRelaxed(Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeIntRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLong(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeLong(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeLongAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLongRelease(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeLongRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLongRelaxed(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeLongRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLong(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeLong(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeLongAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLongRelease(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeLongRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLongRelaxed(Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeLongRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeFloat(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeFloatAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeFloatRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeFloatRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeFloat(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeFloatAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeFloatRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeFloatRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeDouble(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeDoubleAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeDoubleRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return compareAndExchangeDoubleRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeDouble(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeDoubleAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeDoubleRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return weakCompareAndExchangeDoubleRelaxed(o, offset, expected, x) == x;
        }

        gfloat Unsafe::compareAndExchangeFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(compareAndExchangeInt(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::compareAndExchangeFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(compareAndExchangeIntAcquire(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::compareAndExchangeFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(compareAndExchangeIntRelease(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::compareAndExchangeFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(compareAndExchangeIntRelaxed(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::weakCompareAndExchangeFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(weakCompareAndExchangeInt(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::weakCompareAndExchangeFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(weakCompareAndExchangeIntAcquire(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::weakCompareAndExchangeFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(weakCompareAndExchangeIntRelease(o, offset, f2i(expected), f2i(x)));
        }

        gfloat Unsafe::weakCompareAndExchangeFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            return i2f(weakCompareAndExchangeIntRelaxed(o, offset, f2i(expected), f2i(x)));
        }

        gdouble Unsafe::compareAndExchangeDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(compareAndExchangeLong(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::compareAndExchangeDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(compareAndExchangeLongAcquire(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::compareAndExchangeDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(compareAndExchangeLongRelease(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::compareAndExchangeDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(compareAndExchangeLongRelaxed(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::weakCompareAndExchangeDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(weakCompareAndExchangeLong(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::weakCompareAndExchangeDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(weakCompareAndExchangeLongAcquire(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::weakCompareAndExchangeDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(weakCompareAndExchangeLongRelease(o, offset, d2l(expected), d2l(x)));
        }

        gdouble Unsafe::weakCompareAndExchangeDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            return l2d(weakCompareAndExchangeLongRelaxed(o, offset, d2l(expected), d2l(x)));
        }

        gbool Unsafe::compareAndExchangeBoolean(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(compareAndExchangeByte(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::compareAndExchangeBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(compareAndExchangeByteAcquire(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::compareAndExchangeBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(compareAndExchangeByteRelease(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::compareAndExchangeBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(compareAndExchangeByteRelaxed(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::weakCompareAndExchangeBoolean(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(weakCompareAndExchangeByte(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::weakCompareAndExchangeBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(weakCompareAndExchangeByteAcquire(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::weakCompareAndExchangeBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(weakCompareAndExchangeByteRelease(o, offset, b2byte(expected), b2byte(x)));
        }

        gbool Unsafe::weakCompareAndExchangeBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            return b2bool(weakCompareAndExchangeByteRelaxed(o, offset, b2byte(expected), b2byte(x)));
        }

        gchar Unsafe::compareAndExchangeChar(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(compareAndExchangeShort(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::compareAndExchangeCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(compareAndExchangeShortAcquire(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::compareAndExchangeCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(compareAndExchangeShortRelease(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::compareAndExchangeCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(compareAndExchangeShortRelaxed(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::weakCompareAndExchangeChar(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(weakCompareAndExchangeShort(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::weakCompareAndExchangeCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(weakCompareAndExchangeShortAcquire(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::weakCompareAndExchangeCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(weakCompareAndExchangeShortRelease(o, offset, c2s(expected), c2s(x)));
        }

        gchar Unsafe::weakCompareAndExchangeCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            return s2c(compareAndExchangeShortRelaxed(o, offset, c2s(expected), c2s(x)));
        }

        Object &Unsafe::getReferenceVolatile(const Object &o, glong offset) {
            return l2o(ADDRESS_SIZE == 4 ? (glong) getIntVolatile(o, offset) : getLongVolatile(o, offset));
        }

        void Unsafe::putReferenceVolatile(Object &o, glong offset, Object &x) {
            if (ADDRESS_SIZE == 4)
                putIntVolatile(o, offset, (gint) getNativeAddress((Object &) x, 0));
            else
                putLongVolatile(o, offset, getNativeAddress((Object &) x, 0));
        }

        gint Unsafe::getIntVolatile(const Object &o, glong offset) {
            return CORE_EXPR(
                    getInt(o, offset),
                    getInt(o, offset),
                    CORE_BARRIER(getInt(o, offset)),
                    getInt(o, offset)
            );
        }

        void Unsafe::putIntVolatile(Object &o, glong offset, gint x) {
            CORE_EXPR(
                    putInt(o, offset, x),
                    putInt(o, offset, x),
                    CORE_BARRIER(putInt(o, offset, x)),
                    putInt(o, offset, x)
            );
        }

        glong Unsafe::getLongVolatile(const Object &o, glong offset) {
            return CORE_EXPR(
                    getLong(o, offset),
                    getLong(o, offset),
                    CORE_BARRIER(getLong(o, offset)),
                    getLong(o, offset)
            );
        }

        void Unsafe::putLongVolatile(Object &o, glong offset, glong x) {
            CORE_EXPR(
                    putLong(o, offset, x),
                    putLong(o, offset, x),
                    CORE_BARRIER(putLong(o, offset, x)),
                    putLong(o, offset, x)
            );
        }

        gshort Unsafe::getShortVolatile(const Object &o, glong offset) {
            return CORE_EXPR(
                    getShort(o, offset),
                    getShort(o, offset),
                    CORE_BARRIER(getShort(o, offset)),
                    getShort(o, offset)
            );
        }

        void Unsafe::putShortVolatile(Object &o, glong offset, gshort x) {
            CORE_EXPR(
                    putShort(o, offset, x),
                    putShort(o, offset, x),
                    CORE_BARRIER(putShort(o, offset, x)),
                    putShort(o, offset, x)
            );
        }

        gbyte Unsafe::getByteVolatile(const Object &o, glong offset) {
            return CORE_EXPR(
                    getByte(o, offset),
                    getByte(o, offset),
                    CORE_BARRIER(getByte(o, offset)),
                    getByte(o, offset)
            );
        }

        void Unsafe::putByteVolatile(Object &o, glong offset, gbyte x) {
            CORE_EXPR(
                    putByte(o, offset, x),
                    putByte(o, offset, x),
                    CORE_BARRIER(putByte(o, offset, x)),
                    putByte(o, offset, x)
            );
        }

        gbool Unsafe::getBooleanVolatile(const Object &o, glong offset) { return b2bool(getByteVolatile(o, offset)); }

        void Unsafe::putBooleanVolatile(Object &o, glong offset, gbool x) { putByteVolatile(o, offset, b2byte(x)); }

        gchar Unsafe::getCharVolatile(const Object &o, glong offset) { return s2c(getShortVolatile(o, offset)); }

        void Unsafe::putCharVolatile(Object &o, glong offset, gchar x) { putShortVolatile(o, offset, c2s(x)); }

        gfloat Unsafe::getFloatVolatile(const Object &o, glong offset) { return i2f(getIntVolatile(o, offset)); }

        void Unsafe::putFloatVolatile(Object &o, glong offset, gfloat x) { putIntVolatile(o, offset, f2i(x)); }

        gdouble Unsafe::getDoubleVolatile(const Object &o, glong offset) { return l2d(getLongVolatile(o, offset)); }

        void Unsafe::putDoubleVolatile(Object &o, glong offset, gdouble x) { putLongVolatile(o, offset, d2l(x)); }

        Object &Unsafe::getReferenceAcquire(const Object &o, glong offset) {
            return l2o(ADDRESS_SIZE == 4 ? getIntVolatile(o, offset) : getLongVolatile(o, offset));
        }

        gbool Unsafe::getBooleanAcquire(const Object &o, glong offset) { return b2bool(getByteAcquire(o, offset)); }

        gint Unsafe::getIntAcquire(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getIntVolatile(o, offset),
                    __atomic_load_n((gint volatile *) getNativeAddress(o, offset), __ATOMIC_ACQUIRE),
                    getIntVolatile(o, offset),
                    getIntVolatile(o, offset)
            );
        }

        glong Unsafe::getLongAcquire(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getLong(o, offset),
                    __atomic_load_n((glong volatile *) getNativeAddress(o, offset), __ATOMIC_ACQUIRE),
                    getLongVolatile(o, offset),
                    getLongVolatile(o, offset)
            );
        }

        gshort Unsafe::getShortAcquire(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getShort(o, offset),
                    __atomic_load_n((gshort volatile *) getNativeAddress(o, offset), __ATOMIC_ACQUIRE),
                    getShortVolatile(o, offset),
                    getShortVolatile(o, offset)
            );
        }

        gbyte Unsafe::getByteAcquire(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getByte(o, offset),
                    __atomic_load_n((gbyte volatile *) getNativeAddress(o, offset), __ATOMIC_ACQUIRE),
                    getByteVolatile(o, offset),
                    getByteVolatile(o, offset)
            );
        }

        gchar Unsafe::getCharAcquire(const Object &o, glong offset) { return getShortAcquire(o, offset); }

        gfloat Unsafe::getFloatAcquire(const Object &o, glong offset) { return i2f(getIntAcquire(o, offset)); }

        gdouble Unsafe::getDoubleAcquire(const Object &o, glong offset) { return l2d(getLongAcquire(o, offset)); }

        Object &Unsafe::getReferenceRelaxed(const Object &o, glong offset) {
            return l2o(ADDRESS_SIZE == 4 ? getIntVolatile(o, offset) : getLongVolatile(o, offset));
        }

        gint Unsafe::getIntRelaxed(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getInt(o, offset),
                    __atomic_load_n((gint volatile *) getNativeAddress(o, offset), __ATOMIC_RELAXED),
                    getInt(o, offset),
                    getIntVolatile(o, offset)
            );
        }

        glong Unsafe::getLongRelaxed(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getLong(o, offset),
                    __atomic_load_n((glong volatile *) getNativeAddress(o, offset), __ATOMIC_RELAXED),
                    getLong(o, offset),
                    getLongVolatile(o, offset)
            );
        }

        gshort Unsafe::getShortRelaxed(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getShort(o, offset),
                    __atomic_load_n((gshort volatile *) getNativeAddress(o, offset), __ATOMIC_RELAXED),
                    getShort(o, offset),
                    getShortVolatile(o, offset)
            );
        }

        gbyte Unsafe::getByteRelaxed(const Object &o, glong offset) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    getByte(o, offset),
                    __atomic_load_n((gbyte volatile *) getNativeAddress(o, offset), __ATOMIC_RELAXED),
                    getByte(o, offset),
                    getByteVolatile(o, offset)
            );
        }

        gbool Unsafe::getBooleanRelaxed(const Object &o, glong offset) { return b2bool(getByteRelaxed(o, offset)); }

        gchar Unsafe::getCharRelaxed(const Object &o, glong offset) { return getShortRelaxed(o, offset); }

        gfloat Unsafe::getFloatRelaxed(const Object &o, glong offset) { return i2f(getIntRelaxed(o, offset)); }

        gdouble Unsafe::getDoubleRelaxed(const Object &o, glong offset) { return l2d(getLongRelaxed(o, offset)); }

        void Unsafe::putIntRelease(Object &o, glong offset, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((gint volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((gint volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELEASE),
                    putIntVolatile(o, offset, x),
                    putIntVolatile(o, offset, x)
            );
        }

        void Unsafe::putLongRelease(core::Object &o, glong offset, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((glong volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((glong volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELEASE),
                    putLongVolatile(o, offset, x),
                    putLongVolatile(o, offset, x)
            );
        }

        void Unsafe::putShortRelease(core::Object &o, glong offset, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((gshort volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((gshort volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELEASE),
                    putShortVolatile(o, offset, x),
                    putShortVolatile(o, offset, x)
            );
        }

        void Unsafe::putByteRelease(core::Object &o, glong offset, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((gbyte volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((gbyte volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELEASE),
                    putByteVolatile(o, offset, x),
                    putByteVolatile(o, offset, x)
            );
        }

        void Unsafe::putReferenceRelease(core::Object &o, glong offset, core::Object &x) {
            if (ADDRESS_SIZE == 4)
                putIntRelease(o, offset, (gint) o2l(x));
            else
                putLongRelease(o, offset, o2l(x));
        }

        void Unsafe::putCharRelease(core::Object &o, glong offset, gchar x) {
            putShortRelease(o, offset, c2s(x));
        }

        void Unsafe::putBooleanRelease(core::Object &o, glong offset, gbool x) {
            putByteRelease(o, offset, b2byte(x));
        }

        void Unsafe::putFloatRelease(core::Object &o, glong offset, gfloat x) {
            putIntRelease(o, offset, f2i(x));
        }

        void Unsafe::putDoubleRelease(core::Object &o, glong offset, gdouble x) {
            putLongRelease(o, offset, d2l(x));
        }

        void Unsafe::putIntRelaxed(Object &o, glong offset, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((gint volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((gint volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELAXED),
                    putInt(o, offset, x),
                    putIntVolatile(o, offset, x)
            );
        }

        void Unsafe::putLongRelaxed(core::Object &o, glong offset, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((glong volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((glong volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELAXED),
                    putLong(o, offset, x),
                    putLongVolatile(o, offset, x)
            );
        }

        void Unsafe::putShortRelaxed(core::Object &o, glong offset, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((gshort volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((gshort volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELAXED),
                    putShort(o, offset, x),
                    putShortVolatile(o, offset, x)
            );
        }

        void Unsafe::putByteRelaxed(core::Object &o, glong offset, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            CORE_EXPR(
                    __sync_swap((gbyte volatile *) getNativeAddress(o, offset), x),
                    __atomic_store_n((gbyte volatile *) getNativeAddress(o, offset), x, __ATOMIC_RELAXED),
                    putByte(o, offset, x),
                    putByteVolatile(o, offset, x)
            );
        }

        void Unsafe::putReferenceRelaxed(core::Object &o, glong offset, core::Object &x) {
            if (ADDRESS_SIZE == 4)
                putIntRelaxed(o, offset, (gint) o2l(x));
            else
                putLongRelaxed(o, offset, o2l(x));
        }

        void Unsafe::putCharRelaxed(core::Object &o, glong offset, gchar x) {
            putShortRelaxed(o, offset, c2s(x));
        }

        void Unsafe::putBooleanRelaxed(core::Object &o, glong offset, gbool x) {
            putByteRelaxed(o, offset, b2byte(x));
        }

        void Unsafe::putFloatRelaxed(core::Object &o, glong offset, gfloat x) {
            putIntRelaxed(o, offset, f2i(x));
        }

        void Unsafe::putDoubleRelaxed(core::Object &o, glong offset, gdouble x) {
            putLongRelaxed(o, offset, d2l(x));
        }

        gint Unsafe::getAndAddIntAcquire(Object &o, glong offset, gint delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gint volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gint volatile *) getNativeAddress(o, offset), delta, __ATOMIC_ACQUIRE),
                    getAndAddInt(o, offset, delta),
                    [&]() {
                        gint v = {};
                        do {
                            v = getInt(o, offset);
                        } while (!weakCompareAndSetIntAcquire(o, offset, v, (gint) (v + delta)));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndAddLongAcquire(Object &o, glong offset, glong delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((glong volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((glong volatile *) getNativeAddress(o, offset), delta, __ATOMIC_ACQUIRE),
                    getAndAddLong(o, offset, delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLong(o, offset);
                        } while (!weakCompareAndSetLongAcquire(o, offset, v, (glong) (v + delta)));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndAddByteAcquire(Object &o, glong offset, gbyte delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gbyte volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gbyte volatile *) getNativeAddress(o, offset), delta, __ATOMIC_ACQUIRE),
                    getAndAddByte(o, offset, delta),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByte(o, offset);
                        } while (!weakCompareAndSetByteAcquire(o, offset, v, (gbyte) (v + delta)));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndAddShortAcquire(Object &o, glong offset, gshort delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gshort volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gshort volatile *) getNativeAddress(o, offset), delta, __ATOMIC_ACQUIRE),
                    getAndAddShort(o, offset, delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShort(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, v, (gshort) (v + delta)));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndAddCharAcquire(Object &o, glong offset, gchar delta) {
            return s2c(getAndAddShortAcquire(o, offset, c2s(delta)));
        }

        gfloat Unsafe::getAndAddFloatAcquire(Object &o, glong offset, gfloat delta) {
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetIntAcquire(o, offset, expectedBits, f2i(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDoubleAcquire(Object &o, glong offset, gdouble delta) {
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLongAcquire(o, offset, expectedBits, d2l(v + delta)));
            return v;
        }

        gint Unsafe::getAndAddIntRelease(Object &o, glong offset, gint delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gint volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gint volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELEASE),
                    getAndAddInt(o, offset, delata),
                    [&]() {
                        gint v = {};
                        do {
                            v = getInt(o, offset);
                        } while (!weakCompareAndSetIntRelease(o, offset, v, (gint) (v + delta)));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndAddLongRelease(Object &o, glong offset, glong delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((glong volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((glong volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELEASE),
                    getAndAddLong(o, offset, delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLong(o, offset);
                        } while (!weakCompareAndSetLongRelease(o, offset, v, (glong) (v + delta)));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndAddByteRelease(Object &o, glong offset, gbyte delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gbyte volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gbyte volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELEASE),
                    getAndAddByte(o, offset, delta),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByte(o, offset);
                        } while (!weakCompareAndSetLongRelease(o, offset, v, (gbyte) (v + delta)));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndAddShortRelease(Object &o, glong offset, gshort delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gshort volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gshort volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELEASE),
                    getAndAddShort(o, offset, delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShort(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, v, (gshort) (v + delta)));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndAddCharRelease(Object &o, glong offset, gchar delta) {
            return s2c(getAndAddShortRelease(o, offset, c2s(delta)));
        }

        gfloat Unsafe::getAndAddFloatRelease(Object &o, glong offset, gfloat delta) {
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetIntRelease(o, offset, expectedBits, f2i(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDoubleRelease(Object &o, glong offset, gdouble delta) {
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLongRelease(o, offset, expectedBits, d2l(v + delta)));
            return v;
        }

        gint Unsafe::getAndAddIntRelaxed(Object &o, glong offset, gint delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gint volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gint volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELAXED),
                    getAndAddShort(o, offset, delta),
                    [&]() {
                        gint v = {};
                        do {
                            v = getIntRelaxed(o, offset);
                        } while (!weakCompareAndSetIntRelaxed(o, offset, v, (gint) (v + delta)));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndAddLongRelaxed(Object &o, glong offset, glong delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((glong volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((glong volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELAXED),
                    getAndAddLong(o, offset, delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLongRelaxed(o, offset);
                        } while (!weakCompareAndSetLongRelaxed(o, offset, v, (glong) (v + delta)));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndAddByteRelaxed(Object &o, glong offset, gbyte delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gbyte volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gbyte volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELAXED),
                    getAndAddByte(o, offset, delta),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByteRelaxed(o, offset);
                        } while (!weakCompareAndSetLongRelaxed(o, offset, v, (gbyte) (v + delta)));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndAddShortRelaxed(Object &o, glong offset, gshort delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gshort volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gshort volatile *) getNativeAddress(o, offset), delta, __ATOMIC_RELAXED),
                    getAndAddShort(o, offset, delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShortRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, v, (gshort) (v + delta)));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndAddCharRelaxed(Object &o, glong offset, gchar delta) {
            return s2c(getAndAddShortRelaxed(o, offset, c2s(delta)));
        }

        gfloat Unsafe::getAndAddFloatRelaxed(Object &o, glong offset, gfloat delta) {
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntRelaxed(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetIntRelaxed(o, offset, expectedBits, f2i(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDoubleRelaxed(Object &o, glong offset, gdouble delta) {
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongRelaxed(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLongRelaxed(o, offset, expectedBits, d2l(v + delta)));
            return v;
        }

        gint Unsafe::getAndAddInt(Object &o, glong offset, gint delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gint volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gint volatile *) getNativeAddress(o, offset), delta, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(ExchangeAdd)((XADDRESS) getNativeAddress(o, offset), delta),
                    [&]() {
                        gint v = {};
                        do {
                            v = getIntVolatile(o, offset);
                        } while (!weakCompareAndSetInt(o, offset, v, (gint) (v + delta)));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndAddLong(Object &o, glong offset, glong delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((glong volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((glong volatile *) getNativeAddress(o, offset), delta, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(ExchangeAdd64)((__int64 volatile *) getNativeAddress(o, offset), delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLongVolatile(o, offset);
                        } while (!weakCompareAndSetLong(o, offset, v, (glong) (v + delta)));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndAddByte(Object &o, glong offset, gbyte delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gbyte volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gbyte volatile *) getNativeAddress(o, offset), delta, __ATOMIC_SEQ_CST),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByteVolatile(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, v, (gbyte) (v + delta)));
                        return v;
                    }(),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByteVolatile(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, v, (gbyte) (v + delta)));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndAddShort(Object &o, glong offset, gshort delta) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_add((gshort volatile *) getNativeAddress(o, offset), delta),
                    __atomic_fetch_add((gshort volatile *) getNativeAddress(o, offset), delta, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(ExchangeAdd16)((__int16 volatile *) getNativeAddress(o, offset), delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShortVolatile(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, v, (gshort) (v + delta)));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndAddChar(Object &o, glong offset, gchar delta) {
            return s2c(getAndAddShort(o, offset, c2s(delta)));
        }

        gfloat Unsafe::getAndAddFloat(Object &o, glong offset, gfloat delta) {
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntVolatile(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetInt(o, offset, expectedBits, f2i(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDouble(Object &o, glong offset, gdouble delta) {
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongVolatile(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLong(o, offset, expectedBits, d2l(v + delta)));
            return v;
        }

        gint Unsafe::getAndSetIntAcquire(Object &o, glong offset, gint newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gint volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gint volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_ACQUIRE),
                    getAndSetInt(o, offset, delta),
                    [&]() {
                        gint v = {};
                        do {
                            v = getInt(o, offset);
                        } while (!weakCompareAndSetIntAcquire(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndSetLongAcquire(Object &o, glong offset, glong newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((glong volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((glong volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_ACQUIRE),
                    getAndSetLong(o, offset, delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLong(o, offset);
                        } while (!weakCompareAndSetLongAcquire(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndSetByteAcquire(Object &o, glong offset, gbyte newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gbyte volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gbyte volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_ACQUIRE),
                    getAndSetByte(o, offset, delta),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByte(o, offset);
                        } while (!weakCompareAndSetByteAcquire(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndSetShortAcquire(Object &o, glong offset, gshort newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gshort volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gshort volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_ACQUIRE),
                    getAndSetShort(o, offset, delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShort(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndSetCharAcquire(Object &o, glong offset, gchar newValue) {
            return s2c(getAndSetShortAcquire(o, offset, c2s(newValue)));
        }

        gbool Unsafe::getAndSetBooleanAcquire(Object &o, glong offset, gbool newValue) {
            return b2bool(getAndSetByteAcquire(o, offset, b2byte(newValue)));
        }

        gfloat Unsafe::getAndSetFloatAcquire(Object &o, glong offset, gfloat newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetIntAcquire(o, offset, expectedBits, f2i(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDoubleAcquire(Object &o, glong offset, gdouble newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLongAcquire(o, offset, expectedBits, d2l(newValue)));
            return v;
        }

        Object &Unsafe::getAndSetReferenceAcquire(Object &o, glong offset, Object &newValue) {
            return l2o(ADDRESS_SIZE == 4 ?
                       (glong) getAndSetIntAcquire(o, offset, (gint) getNativeAddress(newValue, 0)) :
                       getAndSetLongAcquire(o, offset, getNativeAddress(newValue, 0)));
        }

        gint Unsafe::getAndSetIntRelease(Object &o, glong offset, gint newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gint volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gint volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELEASE),
                    getAndSetInt(o, offset, delta),
                    [&]() {
                        gint v = {};
                        do {
                            v = getInt(o, offset);
                        } while (!weakCompareAndSetIntRelease(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndSetLongRelease(Object &o, glong offset, glong newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((glong volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((glong volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELEASE),
                    getAndSetLong(o, offset, delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLong(o, offset);
                        } while (!weakCompareAndSetLongRelease(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndSetByteRelease(Object &o, glong offset, gbyte newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gbyte volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gbyte volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELEASE),
                    getAndSetByte(o, offset, delta),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByte(o, offset);
                        } while (!weakCompareAndSetByteRelease(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndSetShortRelease(Object &o, glong offset, gshort newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gshort volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gshort volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELEASE),
                    getAndSetShort(o, offset, delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShort(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndSetCharRelease(Object &o, glong offset, gchar newValue) {
            return s2c(getAndSetShortRelease(o, offset, c2s(newValue)));
        }

        gbool Unsafe::getAndSetBooleanRelease(Object &o, glong offset, gbool newValue) {
            return b2bool(getAndSetByteRelease(o, offset, b2byte(newValue)));
        }

        gfloat Unsafe::getAndSetFloatRelease(Object &o, glong offset, gfloat newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetIntRelease(o, offset, expectedBits, f2i(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDoubleRelease(Object &o, glong offset, gdouble newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLongRelease(o, offset, expectedBits, d2l(newValue)));
            return v;
        }

        Object &Unsafe::getAndSetReferenceRelease(Object &o, glong offset, Object &newValue) {
            return l2o(ADDRESS_SIZE == 4 ?
                       (glong) getAndSetIntRelease(o, offset, (gint) getNativeAddress(newValue, 0)) :
                       getAndSetLongRelease(o, offset, getNativeAddress(newValue, 0)));
        }

        gint Unsafe::getAndSetIntRelaxed(Object &o, glong offset, gint newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gint volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gint volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELAXED),
                    getAndSetInt(o, offset, delta),
                    [&]() {
                        gint v = {};
                        do {
                            v = getIntRelaxed(o, offset);
                        } while (!weakCompareAndSetIntRelaxed(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndSetLongRelaxed(Object &o, glong offset, glong newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((glong volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((glong volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELAXED),
                    getAndSetLong(o, offset, delta),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLongRelaxed(o, offset);
                        } while (!weakCompareAndSetLongRelaxed(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndSetByteRelaxed(Object &o, glong offset, gbyte newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gbyte volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gbyte volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELAXED),
                    getAndSetByte(o, offset, delta),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByteRelaxed(o, offset);
                        } while (!weakCompareAndSetByteRelaxed(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndSetShortRelaxed(Object &o, glong offset, gshort newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gshort volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gshort volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_RELAXED),
                    getAndSetShort(o, offset, delta),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShortRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndSetCharRelaxed(Object &o, glong offset, gchar newValue) {
            return s2c(getAndSetShortRelaxed(o, offset, c2s(newValue)));
        }

        gbool Unsafe::getAndSetBooleanRelaxed(Object &o, glong offset, gbool newValue) {
            return b2bool(getAndSetByteRelaxed(o, offset, b2byte(newValue)));
        }

        gfloat Unsafe::getAndSetFloatRelaxed(Object &o, glong offset, gfloat newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntRelaxed(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetIntRelaxed(o, offset, expectedBits, f2i(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDoubleRelaxed(Object &o, glong offset, gdouble newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongRelaxed(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLongRelaxed(o, offset, expectedBits, d2l(newValue)));
            return v;
        }

        Object &Unsafe::getAndSetReferenceRelaxed(Object &o, glong offset, Object &newValue) {
            return l2o(ADDRESS_SIZE == 4 ?
                       (glong) getAndSetIntRelaxed(o, offset, (gint) getNativeAddress(newValue, 0)) :
                       getAndSetLongRelaxed(o, offset, getNativeAddress(newValue, 0)));
        }

        gint Unsafe::getAndSetInt(Object &o, glong offset, gint newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gint volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gint volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Exchange)((XADDRESS) getNativeAddress(o, offset), newValue),
                    [&]() {
                        gint v = {};
                        do {
                            v = getInt(o, offset);
                        } while (!weakCompareAndSetInt(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        glong Unsafe::getAndSetLong(Object &o, glong offset, glong newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((glong volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((glong volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Exchange64)((__int64 volatile *) getNativeAddress(o, offset), newValue),
                    [&]() {
                        glong v = {};
                        do {
                            v = getLong(o, offset);
                        } while (!weakCompareAndSetLong(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gbyte Unsafe::getAndSetByte(Object &o, glong offset, gbyte newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gbyte volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gbyte volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_SEQ_CST),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByte(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, v, newValue));
                        return v;
                    }(),
                    [&]() {
                        gbyte v = {};
                        do {
                            v = getByte(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gshort Unsafe::getAndSetShort(Object &o, glong offset, gshort newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_swap((gshort volatile *) getNativeAddress(o, offset), newValue),
                    __atomic_exchange_n((gshort volatile *) getNativeAddress(o, offset), newValue,
                                        __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Exchange16)((__int16 volatile *) getNativeAddress(o, offset), newValue),
                    [&]() {
                        gshort v = {};
                        do {
                            v = getShort(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, v, newValue));
                        return v;
                    }()
            );
        }

        gchar Unsafe::getAndSetChar(Object &o, glong offset, gchar newValue) {
            return s2c(getAndSetShort(o, offset, c2s(newValue)));
        }

        gbool Unsafe::getAndSetBoolean(Object &o, glong offset, gbool newValue) {
            return b2bool(getAndSetByte(o, offset, b2byte(newValue)));
        }

        gfloat Unsafe::getAndSetFloat(Object &o, glong offset, gfloat newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            gint expectedBits = {};
            gfloat v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = i2f(expectedBits);
            } while (!weakCompareAndSetInt(o, offset, expectedBits, f2i(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDouble(Object &o, glong offset, gdouble newValue) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            glong expectedBits = {};
            gdouble v = {};
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = l2d(expectedBits);
            } while (!weakCompareAndSetLong(o, offset, expectedBits, d2l(newValue)));
            return v;
        }

        Object &Unsafe::getAndSetReference(Object &o, glong offset, Object &newValue) {
            return l2o(ADDRESS_SIZE == 4 ?
                       (glong) getAndSetInt(o, offset, (gint) getNativeAddress(newValue, 0)) :
                       getAndSetLong(o, offset, getNativeAddress(newValue, 0)));
        }

        gbyte Unsafe::getAndBitwiseOrByteAcquire(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseOrByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByteAcquire(o, offset, current, (gbyte) (current | mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseOrShortAcquire(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseOrShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, current, (gshort) (current | mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseOrIntAcquire(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseOrInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, current, (gint) (current | mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseOrLongAcquire(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseOrLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLongAcquire(o, offset, current, (glong) (current | mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseOrBooleanAcquire(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseOrByteAcquire(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseOrCharAcquire(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseOrShortAcquire(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseOrByteRelease(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseOrByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByteRelease(o, offset, current, (gbyte) (current | mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseOrShortRelease(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseOrShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, current, (gshort) (current | mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseOrIntRelease(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseOrInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, current, (gint) (current | mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseOrLongRelease(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseOrLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLongRelease(o, offset, current, (glong) (current | mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseOrBooleanRelease(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseOrByteRelease(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseOrCharRelease(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseOrShortRelease(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseOrByteRelaxed(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseOrByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByteRelaxed(o, offset);
                        } while (!weakCompareAndSetByteRelaxed(o, offset, current, (gbyte) (current | mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseOrShortRelaxed(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseOrShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShortRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gshort) (current | mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseOrIntRelaxed(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseOrInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getIntRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gint) (current | mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseOrLongRelaxed(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseOrLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLongRelaxed(o, offset);
                        } while (!weakCompareAndSetLongRelaxed(o, offset, current, (glong) (current | mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseOrBooleanRelaxed(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseOrByteRelaxed(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseOrCharRelaxed(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseOrShortRelaxed(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseOrByte(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current | mask)));
                        return current;
                    }(),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current | mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseOrShort(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Xor)((__int16 volatile *) getNativeAddress(o, offset), mask),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current | mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseOrInt(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Or)((XADDRESS) getNativeAddress(o, offset), mask),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, current, (gint) (current | mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseOrLong(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Or64)((__int64 volatile *) getNativeAddress(o, offset), mask),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLong(o, offset, current, (glong) (current | mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseOrBoolean(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseOrByte(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseOrChar(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseOrShort(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseAndByteAcquire(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseAndByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByteAcquire(o, offset, current, (gbyte) (current & mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseAndShortAcquire(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseAndShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, current, (gshort) (current & mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseAndIntAcquire(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseAndInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, current, (gint) (current & mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseAndLongAcquire(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseAndLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLongAcquire(o, offset, current, (glong) (current & mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseAndBooleanAcquire(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseAndByteAcquire(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseAndCharAcquire(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseAndShortAcquire(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseAndByteRelease(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseAndByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByteRelease(o, offset, current, (gbyte) (current & mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseAndShortRelease(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseAndShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, current, (gshort) (current & mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseAndIntRelease(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseAndInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, current, (gint) (current & mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseAndLongRelease(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseAndLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLongRelease(o, offset, current, (glong) (current & mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseAndBooleanRelease(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseAndByteRelease(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseAndCharRelease(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseAndShortRelease(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseAndByteRelaxed(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseAndByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByteRelaxed(o, offset);
                        } while (!weakCompareAndSetByteRelaxed(o, offset, current, (gbyte) (current & mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseAndShortRelaxed(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseAndShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShortRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gshort) (current & mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseAndIntRelaxed(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseAndInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getIntRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gint) (current & mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseAndLongRelaxed(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseAndLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLongRelaxed(o, offset);
                        } while (!weakCompareAndSetLongRelaxed(o, offset, current, (glong) (current & mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseAndBooleanRelaxed(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseAndByteRelaxed(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseAndCharRelaxed(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseAndShortRelaxed(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseAndByte(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current & mask)));
                        return current;
                    }(),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current & mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseAndShort(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(And16)((__int16 volatile *) getNativeAddress(o, offset), mask),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current & mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseAndInt(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(And)((XADDRESS) getNativeAddress(o, offset), mask),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, current, (gint) (current & mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseAndLong(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(And64)((__int64 volatile *) getNativeAddress(o, offset), mask),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLong(o, offset, current, (glong) (current & mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseAndBoolean(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseAndByte(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseAndChar(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseAndShort(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseXorByteAcquire(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseXorByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByteAcquire(o, offset, current, (gbyte) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseXorShortAcquire(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseXorShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, current, (gshort) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseXorIntAcquire(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseXorInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShortAcquire(o, offset, current, (gint) (current ^ mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseXorLongAcquire(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_ACQUIRE),
                    getAndBitwiseXorLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLongAcquire(o, offset, current, (glong) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseXorBooleanAcquire(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseXorByteAcquire(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseXorCharAcquire(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseXorShortAcquire(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseXorByteRelease(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseXorByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByte(o, offset);
                        } while (!weakCompareAndSetByteRelease(o, offset, current, (gbyte) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseXorShortRelease(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseXorShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, current, (gshort) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseXorIntRelease(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseXorInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShortRelease(o, offset, current, (gint) (current ^ mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseXorLongRelease(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELEASE),
                    getAndBitwiseXorLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLongRelease(o, offset, current, (glong) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseXorBooleanRelease(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseXorByteRelease(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseXorCharRelease(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseXorShortRelease(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseXorByteRelaxed(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseXorByte(o, offset, delta),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByteRelaxed(o, offset);
                        } while (!weakCompareAndSetByteRelaxed(o, offset, current, (gbyte) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseXorShortRelaxed(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseXorShort(o, offset, delta),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShortRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gshort) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseXorIntRelaxed(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseXorInt(o, offset, delta),
                    [&]() {
                        gint current = {};
                        do {
                            current = getIntRelaxed(o, offset);
                        } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gint) (current ^ mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseXorLongRelaxed(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_RELAXED),
                    getAndBitwiseXorLong(o, offset, delta),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLongRelaxed(o, offset);
                        } while (!weakCompareAndSetLongRelaxed(o, offset, current, (glong) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseXorBooleanRelaxed(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseXorByteRelaxed(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseXorCharRelaxed(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseXorShortRelaxed(o, offset, c2s(mask)));
        }

        gbyte Unsafe::getAndBitwiseXorByte(Object &o, glong offset, gbyte mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gbyte volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gbyte volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByteVolatile(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current ^ mask)));
                        return current;
                    }(),
                    [&]() {
                        gbyte current = {};
                        do {
                            current = getByteVolatile(o, offset);
                        } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gshort Unsafe::getAndBitwiseXorShort(Object &o, glong offset, gshort mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gshort volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gshort volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Xor16)((__int16 volatile *) getNativeAddress(o, offset), mask),
                    [&]() {
                        gshort current = {};
                        do {
                            current = getShort(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gint Unsafe::getAndBitwiseXorInt(Object &o, glong offset, gint mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((gint volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((gint volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Xor)((XADDRESS) getNativeAddress(o, offset), mask),
                    [&]() {
                        gint current = {};
                        do {
                            current = getInt(o, offset);
                        } while (!weakCompareAndSetShort(o, offset, current, (gint) (current ^ mask)));
                        return current;
                    }()
            );
        }

        glong Unsafe::getAndBitwiseXorLong(Object &o, glong offset, glong mask) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_fetch_and_or((glong volatile *) getNativeAddress(o, offset), mask),
                    __atomic_fetch_or((glong volatile *) getNativeAddress(o, offset), mask, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(Xor64)((__int64 volatile *) getNativeAddress(o, offset), mask),
                    [&]() {
                        glong current = {};
                        do {
                            current = getLong(o, offset);
                        } while (!weakCompareAndSetLong(o, offset, current, (glong) (current ^ mask)));
                        return current;
                    }()
            );
        }

        gbool Unsafe::getAndBitwiseXorBoolean(Object &o, glong offset, gbool mask) {
            return b2bool(getAndBitwiseXorByte(o, offset, b2byte(mask)));
        }

        gchar Unsafe::getAndBitwiseXorChar(Object &o, glong offset, gchar mask) {
            return s2c(getAndBitwiseXorShort(o, offset, c2s(mask)));
        }

        Object &Unsafe::compareAndExchangeReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       compareAndExchangeIntAcquire(o, offset, (gint) getNativeAddress(expected, 0),
                                                    (gint) getNativeAddress(x, 0)) :
                       compareAndExchangeLongAcquire(o, offset, getNativeAddress(expected, 0), getNativeAddress(x, 0)));
        }

        Object &Unsafe::compareAndExchangeReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       compareAndExchangeIntRelease(o, offset, (gint) getNativeAddress(expected, 0),
                                                    (gint) getNativeAddress(x, 0)) :
                       compareAndExchangeLongRelease(o, offset, getNativeAddress(expected, 0), getNativeAddress(x, 0)));
        }

        Object &Unsafe::compareAndExchangeReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       compareAndExchangeIntRelaxed(o, offset, (gint) getNativeAddress(expected, 0),
                                                    (gint) getNativeAddress(x, 0)) :
                       compareAndExchangeLongRelaxed(o, offset, getNativeAddress(expected, 0), getNativeAddress(x, 0)));
        }

        Object &Unsafe::compareAndExchangeReference(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       compareAndExchangeInt(o, offset, (gint) getNativeAddress(expected, 0),
                                             (gint) getNativeAddress(x, 0)) :
                       compareAndExchangeLong(o, offset, getNativeAddress(expected, 0), getNativeAddress(x, 0)));
        }

        Object &
        Unsafe::weakCompareAndExchangeReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       weakCompareAndExchangeIntAcquire(o, offset, (gint) getNativeAddress(expected, 0),
                                                        (gint) getNativeAddress(x, 0)) :
                       weakCompareAndExchangeLongAcquire(o, offset, getNativeAddress(expected, 0),
                                                         getNativeAddress(x, 0)));
        }

        Object &
        Unsafe::weakCompareAndExchangeReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       weakCompareAndExchangeIntRelease(o, offset, (gint) getNativeAddress(expected, 0),
                                                        (gint) getNativeAddress(x, 0)) :
                       weakCompareAndExchangeLongRelease(o, offset, getNativeAddress(expected, 0),
                                                         getNativeAddress(x, 0)));
        }

        Object &
        Unsafe::weakCompareAndExchangeReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       weakCompareAndExchangeIntRelaxed(o, offset, (gint) getNativeAddress(expected, 0),
                                                        (gint) getNativeAddress(x, 0)) :
                       weakCompareAndExchangeLongRelaxed(o, offset, getNativeAddress(expected, 0),
                                                         getNativeAddress(x, 0)));
        }

        Object &Unsafe::weakCompareAndExchangeReference(Object &o, glong offset, const Object &expected, Object &x) {
            return l2o(ADDRESS_SIZE == 4 ?
                       weakCompareAndExchangeInt(o, offset, (gint) getNativeAddress(expected, 0),
                                                 (gint) getNativeAddress(x, 0)) :
                       weakCompareAndExchangeLong(o, offset, getNativeAddress(expected, 0), getNativeAddress(x, 0)));
        }

        gint Unsafe::compareAndExchangeIntAcquire(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    compareAndExchangeInt(o, offset, expected, x),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::compareAndExchangeLongAcquire(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    compareAndExchangeLong(o, offset, expected, x),
                    compareAndExchangeLong(o, offset, expected, x)
            );
        }

        gbyte Unsafe::compareAndExchangeByteAcquire(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    compareAndExchangeByte(o, offset, expected, x),
                    compareAndExchangeByte(o, offset, expected, x)
            );
        }

        gshort Unsafe::compareAndExchangeShortAcquire(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange16)((__int16 volatile *) getNativeAddress(o, offset),
                                                                 expected, x),
                    compareAndExchangeShort(o, offset, expected, x)
            );
        }

        gint Unsafe::compareAndExchangeIntRelease(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    compareAndExchangeInt(o, offset, expected, x),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::compareAndExchangeLongRelease(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    compareAndExchangeLong(o, offset, expected, x),
                    compareAndExchangeLong(o, offset, expected, x)
            );
        }

        gbyte Unsafe::compareAndExchangeByteRelease(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    compareAndExchangeByte(o, offset, expected, x),
                    compareAndExchangeByte(o, offset, expected, x)
            );
        }

        gshort Unsafe::compareAndExchangeShortRelease(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    compareAndExchangeShort(o, offset, expected, x),
                    compareAndExchangeShort(o, offset, expected, x)
            );
        }

        gint Unsafe::compareAndExchangeIntRelaxed(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    compareAndExchangeInt(o, offset, expected, x),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::compareAndExchangeLongRelaxed(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    compareAndExchangeLong(o, offset, expected, x),
                    compareAndExchangeLong(o, offset, expected, x)
            );
        }

        gbyte Unsafe::compareAndExchangeByteRelaxed(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    compareAndExchangeByte(o, offset, expected, x),
                    compareAndExchangeByte(o, offset, expected, x)
            );
        }

        gshort Unsafe::compareAndExchangeShortRelaxed(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    compareAndExchangeInt(o, offset, expected, x),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        gint Unsafe::compareAndExchangeInt(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange)((volatile LONG *) getNativeAddress(o, offset), x,
                                                               expected),
                    [&]() -> gint {
                        gint oldVal = getIntRelaxed(o, offset);
                        if (oldVal == expected) {
                            putIntRelaxed(o, offset, x);
                            return x;
                        }
                        return oldVal;
                    }()
            );
        }

        glong Unsafe::compareAndExchangeLong(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange64)((volatile LONG64 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    [&]() -> glong {
                        glong oldVal = getLongRelaxed(o, offset);
                        if (oldVal == expected) {
                            putLongRelaxed(o, offset, x);
                            return x;
                        }
                        return oldVal;
                    }()
            );
        }

        gbyte Unsafe::compareAndExchangeByte(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    [&]() -> gbyte {
                        glong wordOffset = offset & ~3;
                        gint shift = (gint) (offset & 3) << 3;
                        if (BIG_ENDIAN) {
                            shift = 24 - shift;
                        }
                        gint mask = 0xFF << shift;
                        gint maskedExpected = (expected & 0xFF) << shift;
                        gint maskedX = (x & 0xFF) << shift;
                        gint fullWord;
                        do {
                            fullWord = getIntVolatile(o, wordOffset);
                            if ((fullWord & mask) != maskedExpected)
                                return (byte)((fullWord & mask) >> shift);
                        } while (!weakCompareAndSetInt(o, wordOffset,
                                                       fullWord, (fullWord & ~mask) | maskedX));
                        return expected;
                    }(),
                    [&]() -> gbyte {
                        glong wordOffset = offset & ~3;
                        gint shift = (gint) (offset & 3) << 3;
                        if (BIG_ENDIAN) {
                            shift = 24 - shift;
                        }
                        gint mask = 0xFF << shift;
                        gint maskedExpected = (expected & 0xFF) << shift;
                        gint maskedX = (x & 0xFF) << shift;
                        gint fullWord;
                        do {
                            fullWord = getIntVolatile(o, wordOffset);
                            if ((fullWord & mask) != maskedExpected)
                                return (gbyte) ((fullWord & mask) >> shift);
                        } while (!weakCompareAndSetInt(o, wordOffset, fullWord, (fullWord & ~mask) | maskedX));
                        return expected;
                    }()
            );
        }

        gshort Unsafe::compareAndExchangeShort(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, false,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange16)((volatile __int16 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    [&]() -> gshort {
                        if ((offset & 3) == 3) {
                            IllegalArgumentException("Update spans the word, not supported").throws(
                                    __trace("core.util.Unsafe"));
                        }
                        glong wordOffset = offset & ~3;
                        gint shift = (gint) (offset & 3) << 3;
                        if (BIG_ENDIAN) {
                            shift = 16 - shift;
                        }
                        gint mask = 0xFFFF << shift;
                        gint maskedExpected = (expected & 0xFFFF) << shift;
                        gint maskedX = (x & 0xFFFF) << shift;
                        gint fullWord;
                        do {
                            fullWord = getIntVolatile(o, wordOffset);
                            if ((fullWord & mask) != maskedExpected) {
                                return (gshort) ((fullWord & mask) >> shift);
                            }
                        } while (!weakCompareAndSetInt(o, wordOffset, fullWord, (fullWord & ~mask) | maskedX));
                        return expected;
                    }()
            );
        }

        gint Unsafe::weakCompareAndExchangeIntAcquire(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange)((volatile LONG *) getNativeAddress(o, offset), x,
                                                               expected),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::weakCompareAndExchangeLongAcquire(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange64)((volatile LONG64 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    compareAndExchangeLong(o, offset, expected, x)
            );
        }

        gbyte Unsafe::weakCompareAndExchangeByteAcquire(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange)8((volatile char *) getNativeAddress(o, offset),
                                                                (gbyte) x, (gbyte) expected),
                    compareAndExchangeByte(o, offset, expected, x)
            );
        }

        gshort Unsafe::weakCompareAndExchangeShortAcquire(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange16)((volatile __int16 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    compareAndExchangeShort(o, offset, expected, x)
            );
        }

        gint Unsafe::weakCompareAndExchangeIntRelease(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange)((volatile LONG *) getNativeAddress(o, offset), x,
                                                               expected),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::weakCompareAndExchangeLongRelease(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange64)((volatile LONG64 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    compareAndExchangeLong(o, offset, expected, x);
            );
        }

        gbyte Unsafe::weakCompareAndExchangeByteRelease(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    compareAndExchangeByte(o, offset, expected, x),
                    compareAndExchangeByte(o, offset, expected, x)
            );
        }

        gshort Unsafe::weakCompareAndExchangeShortRelease(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELEASE, __ATOMIC_RELAXED),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange16)((volatile __int16 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    weakCompareAndExchangeShort(o, offset, expected, x)
            );
        }

        gint Unsafe::weakCompareAndExchangeIntRelaxed(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange)((volatile LONG *) getNativeAddress(o, offset), x,
                                                               expected),
                    weakCompareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::weakCompareAndExchangeLongRelaxed(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange64)((volatile LONG64 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    weakCompareAndExchangeLong(o, offset, expected, x)
            );
        }

        gbyte Unsafe::weakCompareAndExchangeByteRelaxed(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    compareAndExchangeByte(o, offset, expected, x),
                    compareAndExchangeByte(o, offset, expected, x)
            );
        }

        gshort Unsafe::weakCompareAndExchangeShortRelaxed(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_RELAXED, __ATOMIC_RELAXED),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange16)((volatile __int16 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    compareAndExchangeShort(o, offset, expected, x)
            );
        }

        gint Unsafe::weakCompareAndExchangeInt(core::Object &o, glong offset, gint expected, gint x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gint volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gint *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange)((volatile LONG *) getNativeAddress(o, offset), x,
                                                               expected),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        glong Unsafe::weakCompareAndExchangeLong(core::Object &o, glong offset, glong expected, glong x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((glong volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile glong *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange64)((volatile LONG64 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    compareAndExchangeLong(o, offset, expected, x)
            );
        }

        gbyte Unsafe::weakCompareAndExchangeByte(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gbyte volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gbyte *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    compareAndExchangeInt(o, offset, expected, x),
                    compareAndExchangeInt(o, offset, expected, x)
            );
        }

        gshort Unsafe::weakCompareAndExchangeShort(core::Object &o, glong offset, gshort expected, gshort x) {
            if (checkPointer(o, offset))
                IllegalArgumentException("Invalid input").throws(__trace("core.private.Unsafe"));
            return CORE_EXPR(
                    __sync_val_compare_and_swap((gshort volatile *) getNativeAddress(o, offset), expected, x),
                    __atomic_compare_exchange_n((volatile gshort *) getNativeAddress(o, offset), &expected, x, true,
                                                __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST),
                    CORE_INTERLOCKED_FUNCTION(CompareExchange16)((volatile __int16 *) getNativeAddress(o, offset), x,
                                                                 expected),
                    compareAndExchangeShort(o, offset, expected, x)
            );
        }

        gint Unsafe::toUnsignedInt(gbyte n) { return n & 0xff; }

        gint Unsafe::toUnsignedInt(gshort n) { return n & 0xffff; }

        glong Unsafe::toUnsignedLong(gbyte n) { return n & 0xffL; }

        glong Unsafe::toUnsignedLong(gshort n) { return n & 0xffffL; }

        glong Unsafe::toUnsignedLong(gint n) { return n & 0xffffffffL; }

        gint Unsafe::pickPos(gint top, gint pos) { return Unsafe::BIG_ENDIAN ? top - pos : pos; }

        glong Unsafe::makeLong(gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5, gbyte i6, gbyte i7) {
            return ((toUnsignedLong(i0) << pickPos(56, 0))
                    | (toUnsignedLong(i1) << pickPos(56, 8))
                    | (toUnsignedLong(i2) << pickPos(56, 16))
                    | (toUnsignedLong(i3) << pickPos(56, 24))
                    | (toUnsignedLong(i4) << pickPos(56, 32))
                    | (toUnsignedLong(i5) << pickPos(56, 40))
                    | (toUnsignedLong(i6) << pickPos(56, 48))
                    | (toUnsignedLong(i7) << pickPos(56, 56)));
        }

        glong Unsafe::makeLong(gshort i0, gshort i1, gshort i2, gshort i3) {
            return ((toUnsignedLong(i0) << pickPos(48, 0))
                    | (toUnsignedLong(i1) << pickPos(48, 16))
                    | (toUnsignedLong(i2) << pickPos(48, 32))
                    | (toUnsignedLong(i3) << pickPos(48, 48)));
        }

        glong Unsafe::makeLong(gint i0, gint i1) {
            return (toUnsignedLong(i0) << pickPos(32, 0))
                   | (toUnsignedLong(i1) << pickPos(32, 32));
        }

        gint Unsafe::makeInt(gshort i0, gshort i1) {
            return (toUnsignedInt(i0) << pickPos(16, 0))
                   | (toUnsignedInt(i1) << pickPos(16, 16));
        }

        gint Unsafe::makeInt(gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
            return ((toUnsignedInt(i0) << pickPos(24, 0))
                    | (toUnsignedInt(i1) << pickPos(24, 8))
                    | (toUnsignedInt(i2) << pickPos(24, 16))
                    | (toUnsignedInt(i3) << pickPos(24, 24)));
        }

        gshort Unsafe::makeShort(gbyte i0, gbyte i1) {
            return (gshort) ((toUnsignedInt(i0) << pickPos(8, 0))
                             | (toUnsignedInt(i1) << pickPos(8, 8)));
        }

        gbyte Unsafe::pick(gbyte le, gbyte be) { return BIG_ENDIAN ? be : le; }

        gshort Unsafe::pick(gshort le, gshort be) { return BIG_ENDIAN ? be : le; }

        gint Unsafe::pick(gint le, gint be) { return BIG_ENDIAN ? be : le; }

        void Unsafe::putLongParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5,
                                  gbyte i6, gbyte i7) {
            putByte(o, offset + 0, pick(i0, i7));
            putByte(o, offset + 1, pick(i1, i6));
            putByte(o, offset + 2, pick(i2, i5));
            putByte(o, offset + 3, pick(i3, i4));
            putByte(o, offset + 4, pick(i4, i3));
            putByte(o, offset + 5, pick(i5, i2));
            putByte(o, offset + 6, pick(i6, i1));
            putByte(o, offset + 7, pick(i7, i0));
        }

        void Unsafe::putLongParts(Object &o, glong offset, gshort i0, gshort i1, gshort i2, gshort i3) {
            putShort(o, offset + 0, pick(i0, i3));
            putShort(o, offset + 2, pick(i1, i2));
            putShort(o, offset + 4, pick(i2, i1));
            putShort(o, offset + 6, pick(i3, i0));
        }

        void Unsafe::putLongParts(Object &o, glong offset, gint i0, gint i1) {
            putInt(o, offset + 0, pick(i0, i1));
            putInt(o, offset + 4, pick(i1, i0));
        }

        void Unsafe::putIntParts(Object &o, glong offset, gshort i0, gshort i1) {
            putShort(o, offset + 0, pick(i0, i1));
            putShort(o, offset + 2, pick(i1, i0));
        }

        void Unsafe::putIntParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
            putByte(o, offset + 0, pick(i0, i3));
            putByte(o, offset + 1, pick(i1, i2));
            putByte(o, offset + 2, pick(i2, i1));
            putByte(o, offset + 3, pick(i3, i0));
        }

        void Unsafe::putShortParts(Object &o, glong offset, gbyte i0, gbyte i1) {
            putByte(o, offset + 0, pick(i0, i1));
            putByte(o, offset + 1, pick(i1, i0));
        }

        gchar Unsafe::convEndian(gbool big, gchar n) { return big == BIG_ENDIAN ? n : Character::reverseBytes(n); }

        gshort Unsafe::convEndian(gbool big, gshort n) { return big == BIG_ENDIAN ? n : Short::reverseBytes(n); }

        gint Unsafe::convEndian(gbool big, gint n) { return big == BIG_ENDIAN ? n : Integer::reverseBytes(n); }

        glong Unsafe::convEndian(gbool big, glong n) { return big == BIG_ENDIAN ? n : Long::reverseBytes(n); }

        gbyte Unsafe::b2byte(gbool b) { return b ? 1 : 0; }

        gbool Unsafe::b2bool(gbyte b) { return b != 0; }

        gshort Unsafe::c2s(gchar c) { return (gshort) c; }

        gchar Unsafe::s2c(gshort s) { return (gchar) s; }

        gint Unsafe::f2i(gfloat f) { return Float::toIntBits(f); }

        glong Unsafe::d2l(gdouble d) { return Double::toLongBits(d); }

        gfloat Unsafe::i2f(gint i) { return Float::fromIntBits(i); }

        gdouble Unsafe::l2d(glong l) { return Double::fromLongBits(l); }

        glong Unsafe::o2l(const Object &o) { return &o == &null ? 0 : (glong) &o; }

        Object &Unsafe::l2o(glong l) { return l == 0 ? (Object &) null : *((Object *) l); }

        gbool Unsafe::is32Bits(glong size) { return (size >> 32) == 0; }

        gbool Unsafe::checkSize(glong size) {
            if (ADDRESS_SIZE == 4) {
                if (!is32Bits(size)) {
                    return false;
                }
            } else if (size < 0) {
                return false;
            }
            return true;
        }

        gbool Unsafe::checkNativeAddress(glong address) {
            if (ADDRESS_SIZE == 4) {
                // Accept both zero and sign extended pointers. A valid
                // pointer will, after the +1 below, either have produced
                // the value 0x0 or 0x1. Masking off the low bit allows
                // for testing against 0.
                if ((((address >> 32) + 1) & ~1) != 0) {
                    return false;
                }
            }
            return true;
        }

        gbool Unsafe::checkOffset(const Object &obj, glong offset) {
            CORE_IGNORE(obj);
            if (ADDRESS_SIZE == 4) {
                // Note: this will also check for negative offsets
                if (!is32Bits(offset)) {
                    return false;
                }
            } else if (offset < 0) {
                return false;
            }
            return true;
        }

        gbool Unsafe::checkPointer(const Object &o, glong offset) {
            return &o == &null ? checkNativeAddress(offset) : checkOffset(o, offset);
        }

        glong Unsafe::getNativeAddress(const Object &o, glong offset) {
            if (&o == &null) {
                return offset;
            } else if (offset == 0) {
                return (glong) &o;
            } else {
                glong baseOffset = 0;
                glong scaleOffset = 0;
                glong scale = 0;
                if (Class<BooleanArray>::hasInstance(o)) {
                    baseOffset = ARRAY_BOOLEAN_BASE_OFFSET;
                    scale = ARRAY_BOOLEAN_INDEX_SCALE;
                } else if (Class<ByteArray>::hasInstance(o)) {
                    baseOffset = ARRAY_BYTE_BASE_OFFSET;
                    scale = ARRAY_BYTE_INDEX_SCALE;
                } else if (Class<ShortArray>::hasInstance(o)) {
                    baseOffset = ARRAY_SHORT_BASE_OFFSET;
                    scale = ARRAY_SHORT_INDEX_SCALE;
                } else if (Class<CharArray>::hasInstance(o)) {
                    baseOffset = ARRAY_CHAR_BASE_OFFSET;
                    scale = ARRAY_CHAR_INDEX_SCALE;
                } else if (Class<IntArray>::hasInstance(o)) {
                    baseOffset = ARRAY_INT_BASE_OFFSET;
                    scale = ARRAY_INT_INDEX_SCALE;
                } else if (Class<FloatArray>::hasInstance(o)) {
                    baseOffset = ARRAY_FLOAT_BASE_OFFSET;
                    scale = ARRAY_FLOAT_INDEX_SCALE;
                } else if (Class<LongArray>::hasInstance(o)) {
                    baseOffset = ARRAY_LONG_BASE_OFFSET;
                    scale = ARRAY_LONG_INDEX_SCALE;
                } else if (Class<DoubleArray>::hasInstance(o)) {
                    baseOffset = ARRAY_DOUBLE_BASE_OFFSET;
                    scale = ARRAY_DOUBLE_INDEX_SCALE;
                } else if (Class<ObjectArray>::hasInstance(o)) {
                    baseOffset = ARRAY_REFERENCE_BASE_OFFSET;
                    scale = ARRAY_REFERENCE_INDEX_SCALE;
                } else {
                    baseOffset = 0;
                    scale = 1;
                }
                scaleOffset = offset - baseOffset;
                if (scaleOffset % scale != 0) {
                    IllegalArgumentException("Illegal offset").throws(__trace("core.private.Unsafe"));
                }
                glong const baseAddress = baseOffset == 0 ? (glong) &o
                                                    : getAddress(null, (glong) &o + baseOffset);
                return baseAddress + scaleOffset;
            }
        }

        glong Unsafe::alignToHeapWordSize(glong sizeInBytes) {
            if (sizeInBytes >= 0) {
                return (sizeInBytes + ADDRESS_SIZE - 1) & ~(ADDRESS_SIZE - 1);
            } else {
                return -1;
            }
        }

    }

} // core
