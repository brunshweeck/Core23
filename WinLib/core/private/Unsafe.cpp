//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
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

namespace core {
    namespace native {

        namespace {
            HANDLE privateHeap[1024] = {};
        }

        glong Unsafe::allocateMemoryImpl(glong sizeInBytes) {
            if (sizeInBytes == 0)
                return 0;
//            HLOCAL handle = LocalAlloc(LMEM_ZEROINIT | LMEM_FIXED, (SIZE_T) sizeInBytes);
//            if (handle != NULL) {
//                return (glong) handle;
//            }
//            handle = LocalAlloc(LMEM_ZEROINIT | LMEM_MOVEABLE, (SIZE_T) sizeInBytes);
//            if (handle != NULL) {
//                glong const address = (glong) LocalLock(handle);
//                if(address != 0) {
//                    return address;
//                }
//            }
            HANDLE handle = NULL;
            if (privateHeap[1] == NULL) {
                // start with private heap
                handle = privateHeap[1] = HeapCreate(HEAP_GENERATE_EXCEPTIONS, sizeInBytes, 0);
                if (handle == NULL) {
                    handle = privateHeap[1] = HeapCreate(HEAP_GENERATE_EXCEPTIONS, sizeInBytes,
                                                         sizeInBytes + ADDRESS_SIZE);
                }
            }
            else{
                handle = privateHeap[1];
            }
            if (handle != NULL) {
                HeapLock(handle);
                LPVOID ptr = HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeInBytes + 1ULL);
                if (ptr != NULL) {
                    HeapUnlock(handle);
                    return (glong) ptr;
                }
                HeapUnlock(handle);
            }
            if (privateHeap[0] == NULL) {
                handle = privateHeap[0] = GetProcessHeap();
            }
            if (handle != NULL) {
                HeapLock(handle);
                LPVOID ptr = HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeInBytes + 1ULL);
                if (ptr != NULL) {
                    HeapUnlock(handle);
                    return (glong) ptr;
                }
                HeapUnlock(handle);
            }
            for (HANDLE &heap: privateHeap) {
                if (heap != NULL) {
                    handle = heap;
                    HeapLock(heap);
                    LPVOID ptr = HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeInBytes + 1ULL);
                    if (ptr != NULL) {
                        HeapUnlock(heap);
                        return (glong) ptr;
                    }
                    HeapUnlock(heap);
                } else {
                    handle = heap = HeapCreate(0, sizeInBytes, 0);
                    if (handle == NULL) {
                        handle = heap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, sizeInBytes, sizeInBytes + ADDRESS_SIZE);
                    }
                    if (handle == NULL)
                        return 0;
                    HeapLock(heap);
                    LPVOID ptr = HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeInBytes + 1ULL);
                    if (ptr != NULL) {
                        HeapUnlock(heap);
                        return (glong) ptr;
                    }
                    HeapUnlock(heap);
                    return 0;
                }
            }
            return 0;
        }

        glong Unsafe::reallocateMemoryImpl(glong address, glong sizeInBytes) {
            for (HANDLE &heap: privateHeap) {
                if (heap == NULL)
                    break;
                HeapLock(heap);
                LPVOID ptr = HeapReAlloc(heap, HEAP_ZERO_MEMORY, (LPVOID) address, sizeInBytes + 1ULL);
                if (ptr != NULL) {
                    HeapUnlock(heap);
                    return (glong) ptr;
                }
                HeapUnlock(heap);
            }
            return 0;
        }

        void Unsafe::freeMemoryImpl(glong address) {
            for(HANDLE &heap: privateHeap) {
                if (heap == NULL) {
                    if(heap == privateHeap[0])
                        continue;
                    break;
                }
                HeapLock(heap);
                // check if the address is valid for specified heap
                if(HeapValidate(heap, 0, (LPVOID) address) == 0) {
                    HeapUnlock(heap);
                    continue;
                }
                if(HeapFree(heap, 0, (LPVOID) address) != 0){
                    HeapUnlock(heap);
                    return;
                }
                HeapUnlock(heap);
            }
        }

        void Unsafe::setMemoryImpl(glong address, glong sizeInBytes, gbyte value) {
            if (value == 0)
                ZeroMemory ((HLOCAL) address, sizeInBytes);
            else
                FillMemory ((HLOCAL) address, sizeInBytes, value);
        }

        void Unsafe::copyMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes) {
            CopyMemory((HLOCAL) destAddress, (HLOCAL) srcAddress, sizeInBytes);
        }

        void Unsafe::copySwapMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes, glong  /*elemSize*/) {
            MoveMemory((HLOCAL) destAddress, (HLOCAL) srcAddress, sizeInBytes);
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
