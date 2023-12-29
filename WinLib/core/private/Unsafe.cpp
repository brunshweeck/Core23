//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/Float.h>
#include <core/Double.h>
#include <Windows.h>

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

using namespace core;

namespace {
    CORE_FAST gbyte b2byte(gbool b) { return b ? 1 : 0; }

    CORE_FAST gbool b2bool(gbyte b) { return b != 0; }

    CORE_FAST gshort c2s(gchar c) { return (gshort) c; }

    CORE_FAST gchar s2c(gshort s) { return (gchar) s; }

    gint f2i(gfloat f) { return Float::toIntBits(f); }

    glong d2l(gdouble d) { return Double::toLongBits(d); }

    gfloat i2f(gint i) { return Float::fromIntBits(i); }

    gdouble l2d(glong l) { return Double::fromLongBits(l); }

    glong o2l(const Object &o) { return o == null ? 0 : (glong) &o; }

    Object &l2o(glong l) { return l == 0 ? (Object &) null : *((Object *) l); }
}

namespace core {
    namespace native {

        namespace {

            gbool is32Bits(glong size) { return (size >> 32) == 0; }

            gbool checkSize(glong size) {
                if (Unsafe::ADDRESS_SIZE == 4) {
                    if (!is32Bits(size))
                        return false;
                } else if (size < 0)
                    return false;
                return true;
            }

            gbool checkNativeAddress(glong address) {
                if (Unsafe::ADDRESS_SIZE == 4) {
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

            gbool checkOffset(const Object & /*o*/, glong offset) {
                if (Unsafe::ADDRESS_SIZE == 4) {
                    // Note: this will also check for negative offsets
                    if (!is32Bits(offset)) {
                        return false;
                    }
                } else if (offset < 0) {
                    return false;
                }
                return true;
            }

            gbool checkPointer(const Object &o, glong offset) {
                return null == o ? checkNativeAddress(offset) : checkOffset(o, offset);
            }

            glong getNativeAddress(const Object &o, glong offset) { return (glong) &o + offset; }

            /**
             * Round up allocation size to a multiple of HeapWordSize.
             */
            glong alignToHeapWordSize(glong bytes) {
                if (bytes >= 0) {
                    return (bytes + Unsafe::ADDRESS_SIZE - 1) & ~(Unsafe::ADDRESS_SIZE - 1);
                } else {
                    return -1;
                }
            }
        };

        glong Unsafe::allocateMemoryImpl(glong sizeInBytes) {
            try {
                if (sizeInBytes % 8 == 0)
                    return (glong) new glong[sizeInBytes >> 3];
                if (sizeInBytes % 4 == 0)
                    return (glong) new gint[sizeInBytes >> 2];
                if (sizeInBytes % 2 == 0)
                    return (glong) new gshort[sizeInBytes >> 1];
                return (glong) new gbyte[sizeInBytes >> 0];
            } catch (...) { return 0L; }
        }

        glong Unsafe::reallocateMemoryImpl(glong address, glong sizeInBytes) {
            return (glong) LocalReAlloc((HLOCAL) address, sizeInBytes, 0);
        }

        void Unsafe::freeMemoryImpl(glong address) {
            delete[] (gbyte *) address;
        }

        void Unsafe::setMemoryImpl(glong address, glong sizeInBytes, gbyte value) {
            FillMemory((void *) address, sizeInBytes, value);
        }

        void Unsafe::copyMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes) {
            CopyMemory((void *) destAddress, (void *) srcAddress, sizeInBytes);
        }

        void Unsafe::copySwapMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes, glong  /*elemSize*/) {
            MoveMemory((void *) destAddress, (void *) srcAddress, sizeInBytes);
        }

        void Unsafe::loadFence() {
            CORE_EXPR(
                    __sync_synchronize(),
                    __atomic_thread_fence(__ATOMIC_ACQUIRE),
                    CORE_BARRIER({}),
                    LoadFence()
            );
        }

        void Unsafe::storeFence() {
            CORE_EXPR(
                    __sync_synchronize(),
                    __atomic_thread_fence(__ATOMIC_RELEASE),
                    CORE_BARRIER({}),
                    StoreFence()
            );
        }

        void Unsafe::fullFence() {
            CORE_EXPR(
                    __sync_synchronize(),
                    __atomic_thread_fence(__ATOMIC_ACQUIRE + __ATOMIC_RELEASE),
                    []() {
#if defined(_M_IX86) || defined(_M_X64)
                        CORE_BARRIER({})
                        long volatile Guard;
                        InterlockedIncrement(&Guard);
                        CORE_BARRIER({});
#elif defined(_M_ARM) || defined(_M_ARM64)
                        CORE_BARRIER({});
#else
                        // error
#endif
                    }(),
                    FastFence()
            );
        }

    }
}
