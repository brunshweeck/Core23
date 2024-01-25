//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <string.h>
#include <stdlib.h>
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>

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


namespace core {
    namespace native {


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
            return (glong) realloc((void *) address, sizeInBytes);
        }

        void Unsafe::freeMemoryImpl(glong address) {
            delete[] (gbyte *) address;
        }

        void Unsafe::setMemoryImpl(glong address, glong sizeInBytes, gbyte value) {
            memset((void *) address, value, sizeInBytes);
        }

        void Unsafe::copyMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes) {
            memcpy((void *) destAddress, (void *) srcAddress, sizeInBytes);
        }

        void Unsafe::copySwapMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes, glong  /*elemSize*/) {
            memmove((void *) destAddress, (void *) srcAddress, sizeInBytes);
        }

        void Unsafe::loadFence() {
            CORE_EXPR(
                    __sync_synchronize(),
                    __atomic_thread_fence(__ATOMIC_ACQUIRE),
                    fullFence(),
                    fullFence()
            );
        }

        void Unsafe::storeFence() {
            CORE_EXPR(
                    __sync_synchronize(),
                    __atomic_thread_fence(__ATOMIC_RELEASE),
                    fullFence(),
                    fullFence()
            );
        }

        void Unsafe::fullFence() {
            CORE_EXPR(
                    __sync_synchronize(),
                    __atomic_thread_fence(__ATOMIC_ACQUIRE + __ATOMIC_RELEASE),,
            );
        }


    }
}

