//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/Double.h>
#include <core/Float.h>
#include <Windows.h>

namespace core {
    namespace native {

        interface UnsafeImpl {

            static gbool is32Bits(glong size) {
                return (size >> 32) == 0;
            }

            static gbool checkSize(glong size) {
                if (Unsafe::ADDRESS_SIZE == 4) {
                    if (!is32Bits(size))
                        return false;
                } else if (size < 0)
                    return false;
                return true;
            }

            static gbool checkNativeAddress(glong address) {
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

            static gbool checkOffset(const Object &o, glong offset) {
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

            static gbool checkPointer(const Object &o, glong offset) {
                return null == o ? checkNativeAddress(offset) : checkOffset(o, offset);
            }

            static glong getNativeAddress(const Object &o, glong offset) { return (glong) &o + offset; }

            /**
             * Round up allocation size to a multiple of HeapWordSize.
             */
            static glong alignToHeapWordSize(glong bytes) {
                if (bytes >= 0) {
                    return (bytes + Unsafe::ADDRESS_SIZE - 1) & ~(Unsafe::ADDRESS_SIZE - 1);
                } else {
                    return -1;
                }
            }
        };


        glong Unsafe::allocateMemoryImpl(glong sizeInBytes) {
            try {
                return (glong) new gbyte[sizeInBytes];
            } catch (...) { return 0L; }
        }

        glong Unsafe::reallocateMemoryImpl(glong address, glong sizeInBytes) {
            return (glong) LocalReAlloc((HLOCAL) address, sizeInBytes, 0);
        }

        void Unsafe::freeMemoryImpl(glong address) {
            delete[] (gbyte *) address;
        }

        void Unsafe::setMemoryImpl(glong address, glong sizeInBytes, gbyte value) {
            if (address != 0) {
                glong i = 0;
                if (sizeInBytes >= 8) {
                    glong x = (glong) value;
                    glong v = x << 0 |
                              x << 8 |
                              x << 16 |
                              x << 24 |
                              x << 32 |
                              x << 40 |
                              x << 48 |
                              x << 56;
                    for (; sizeInBytes >= 8; i += 8) {
                        putLong(address + i, x);
                        sizeInBytes -= 8;
                    }
                }
                if (sizeInBytes >= 4) {
                    gint x = (gint) value;
                    gint v = x << 0 |
                             x << 8 |
                             x << 16 |
                             x << 24;
                    for (; sizeInBytes >= 4; i += 4) {
                        putInt(address + i, x);
                        sizeInBytes -= 4;
                    }
                }
                if (sizeInBytes >= 2) {
                    gshort x = (gshort) value;
                    gshort v = x << 0 |
                               x << 8 |
                               x << 16 |
                               x << 24;
                    for (; sizeInBytes >= 2; i += 2) {
                        putShort(address + i, x);
                        sizeInBytes -= 2;
                    }
                }
                if (sizeInBytes >= 1) {
                    putByte(address + i, value);
                    sizeInBytes -= 1;
                }
            }
        }

        void Unsafe::copyMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes) {
            if (srcAddress != 0 && destAddress != 0) {
                glong i = 0;
                if (sizeInBytes >= 8) {
                    glong v = getLong(srcAddress + i);
                    for (; sizeInBytes >= 8; i += 8) {
                        putLong(destAddress + i, v);
                        sizeInBytes -= 8;
                    }
                }
                if (sizeInBytes >= 4) {
                    gint v = getInt(srcAddress + i);
                    for (; sizeInBytes >= 4; i += 4) {
                        putInt(destAddress + i, v);
                        sizeInBytes -= 4;
                    }
                }
                if (sizeInBytes >= 2) {
                    gshort v = getShort(srcAddress + i);
                    for (; sizeInBytes >= 2; i += 2) {
                        putShort(destAddress + i, v);
                        sizeInBytes -= 2;
                    }
                }
                if (sizeInBytes >= 1) {
                    putByte(destAddress + i, getByte(srcAddress + i));
                    sizeInBytes -= 1;
                }
            }
        }

        void Unsafe::copySwapMemoryImpl(glong srcAddress, glong destAddress, glong sizeInBytes, glong elemSize) {
            if (srcAddress != 0 && destAddress != 0) {
                switch (elemSize) {
                    case 1:
                        if (sizeInBytes > 1000) {
                            if (sizeInBytes % 8 == 0)
                                return copySwapMemoryImpl(srcAddress, destAddress, sizeInBytes >> 3, 8);
                            if (sizeInBytes % 4 == 0)
                                return copySwapMemoryImpl(srcAddress, destAddress, sizeInBytes >> 2, 4);
                            if (sizeInBytes % 2 == 0)
                                return copySwapMemoryImpl(srcAddress, destAddress, sizeInBytes >> 1, 2);
                        }
                        for (glong i = 0; i < sizeInBytes; ++i) putByte(destAddress + i, getByte(srcAddress + i));
                        break;
                    case 2:
                        if ((sizeInBytes >> 1) > 1000) {
                            if (sizeInBytes % 8 == 0)
                                return copySwapMemoryImpl(srcAddress, destAddress, sizeInBytes >> 3, 8);
                            if (sizeInBytes % 4 == 0)
                                return copySwapMemoryImpl(srcAddress, destAddress, sizeInBytes >> 2, 4);
                        }
                        for (glong i = 0; i < sizeInBytes; i += 2) putShort(destAddress + i, getShort(srcAddress + i));
                        break;
                    case 4:
                        if ((sizeInBytes >> 2) > 1000 && sizeInBytes % 8 == 0)
                            return copySwapMemoryImpl(srcAddress, destAddress, sizeInBytes >> 3, 8);
                        for (glong i = 0; i < sizeInBytes; i += 4) putInt(destAddress + i, getInt(srcAddress + i));
                        break;
                    case 8:
                    default:
                        for (glong i = 0; i < sizeInBytes; i += 8) putLong(destAddress + i, getLong(srcAddress + i));
                        break;
                }
            }
        }

        void Unsafe::loadFence() {
            LoadFence();
        }

        void Unsafe::storeFence() {
            StoreFence();
        }

        void Unsafe::fullFence() {
            FastFence();
        }

        Object &Unsafe::compareAndExchangeReference(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            compareAndExchangeInt(o, offset, (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                  (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            compareAndExchangeLong(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                   UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::compareAndExchangeReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            compareAndExchangeIntAcquire(o, offset, (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                         (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            compareAndExchangeLongAcquire(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                          UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::compareAndExchangeReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            compareAndExchangeIntRelaxed(o, offset, (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                         (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            compareAndExchangeLongRelaxed(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                          UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::compareAndExchangeReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            compareAndExchangeIntRelease(o, offset, (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                         (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            compareAndExchangeLongRelease(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                          UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::weakCompareAndExchangeReference(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            weakCompareAndExchangeInt(o, offset, (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                      (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            weakCompareAndExchangeLong(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                       UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::weakCompareAndExchangeReferenceAcquire(Object &o, glong offset, const Object &expected,
                                                               Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            weakCompareAndExchangeIntAcquire(o, offset,
                                                             (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                             (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            weakCompareAndExchangeLongAcquire(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                              UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::weakCompareAndExchangeReferenceRelaxed(Object &o, glong offset, const Object &expected,
                                                               Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            weakCompareAndExchangeIntRelaxed(o, offset,
                                                             (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                             (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            weakCompareAndExchangeLongRelaxed(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                              UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::weakCompareAndExchangeReferenceRelease(Object &o, glong offset, const Object &expected,
                                                               Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            weakCompareAndExchangeIntRelease(o, offset,
                                                             (gint) UnsafeImpl::getNativeAddress(expected, 0),
                                                             (gint) UnsafeImpl::getNativeAddress(x, 0)) :
                            weakCompareAndExchangeLongRelease(o, offset, UnsafeImpl::getNativeAddress(expected, 0),
                                                              UnsafeImpl::getNativeAddress(x, 0));
            return address == 0 ? null : *(Object *) address;
        }

        gint Unsafe::compareAndExchangeInt(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, false, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, false, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchange((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), (LONG) x,
                                              (LONG) expected);
        }

        gint Unsafe::compareAndExchangeIntAcquire(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, false, 2, 2) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, false, 2, 2) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchangeAcquire((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset),
                                                     (LONG) x, (LONG) expected);
        }

        gint Unsafe::compareAndExchangeIntRelease(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, false, 3, 0) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, false, 3, 0) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchangeRelease((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset),
                                                     (LONG) x,
                                                     (LONG) expected);
        }

        gint Unsafe::compareAndExchangeIntRelaxed(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, false, 0, 0) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, false, 0, 0) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchange((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), (LONG) x,
                                              (LONG) expected);
        }

        gint Unsafe::weakCompareAndExchangeInt(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchange((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), (LONG) x,
                                              (LONG) expected);
        }

        gint Unsafe::weakCompareAndExchangeIntAcquire(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 2, 2) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 2, 2) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchangeAcquire((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset),
                                                     (LONG) x, (LONG) expected);
        }

        gint Unsafe::weakCompareAndExchangeIntRelease(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 3, 0) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 3, 0) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchangeRelease((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset),
                                                     (LONG) x, (LONG) expected);
        }

        gint Unsafe::weakCompareAndExchangeIntRelaxed(core::Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 0, 0) ?
            expected : getInt(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile gint *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 0, 0) ?
            expected : getInt(o, offset);
#endif
            return InterlockedCompareExchange((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), (LONG) x,
                                              (LONG) expected);
        }

        glong Unsafe::compareAndExchangeLong(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchange64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), (LONG64) x,
                                                (LONG64) expected);
        }

        glong Unsafe::compareAndExchangeLongAcquire(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchangeAcquire64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset),
                                                       (LONG64) x, (LONG64) expected);
        }

        glong Unsafe::compareAndExchangeLongRelease(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchangeRelease64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset),
                                                       (LONG64) x, (LONG64) expected);
        }

        glong Unsafe::compareAndExchangeLongRelaxed(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchange64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), (LONG64) x,
                                                (LONG64) expected);
        }

        glong Unsafe::weakCompareAndExchangeLong(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchange64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), (LONG64) x,
                                                (LONG64) expected);
        }

        glong Unsafe::weakCompareAndExchangeLongAcquire(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchangeAcquire64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset),
                                                       (LONG64) x, (LONG64) expected);
        }

        glong Unsafe::weakCompareAndExchangeLongRelease(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchangeRelease64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset),
                                                       (LONG64) x,
                                                       (LONG64) expected);
        }

        glong Unsafe::weakCompareAndExchangeLongRelaxed(core::Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_compare_exchange_n)
            return __atomic_compare_exchange_n((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, x, true, 4, 2) ?
            expected : getLong(o, offset);
#elif __has_builtin(__atomic_compare_exchange)
            return __atomic_compare_exchange((volatile glong *) UnsafeImpl::getNativeAddress(o, offset), &expected, &x, true, 4, 2) ?
            expected : getLong(o, offset);
#endif
            return InterlockedCompareExchange64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), (LONG64) x,
                                                (LONG64) expected);
        }

        gbyte Unsafe::compareAndExchangeByte(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));

            glong wordOffset = offset & ~3;
            gint shift = (gint) (offset & 3) << 3;
            if (BIG_ENDIAN) {
                shift = 24 - shift;
            }
            gint mask = 0xFF << shift;
            gint maskedExpected = (expected & 0xFF) << shift;
            gint maskedX = (x & 0xFF) << shift;
            gint fullWord = {};
            do {
                fullWord = getIntVolatile(o, wordOffset);
                if ((fullWord & mask) != maskedExpected)
                    return (byte) ((fullWord & mask) >> shift);
            } while (!weakCompareAndSetInt(o, wordOffset, fullWord, (fullWord & ~mask) | maskedX));
            return expected;
        }

        gbyte Unsafe::compareAndExchangeByteAcquire(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByte(o, offset, expected, x);
        }

        gbyte Unsafe::compareAndExchangeByteRelease(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByte(o, offset, expected, x);
        }

        gbyte Unsafe::compareAndExchangeByteRelaxed(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByte(o, offset, expected, x);
        }

        gbyte Unsafe::weakCompareAndExchangeByte(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByte(o, offset, expected, x);
        }

        gbyte Unsafe::weakCompareAndExchangeByteAcquire(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByte(o, offset, expected, x);
        }

        gbyte Unsafe::weakCompareAndExchangeByteRelease(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByte(o, offset, expected, x);
        }

        gbyte Unsafe::weakCompareAndExchangeByteRelaxed(core::Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByte(o, offset, expected, x);
        }

        gshort Unsafe::compareAndExchangeShort(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return InterlockedCompareExchange16((volatile SHORT *) UnsafeImpl::getNativeAddress(o, offset), (SHORT) x,
                                                (SHORT) expected);
//            if ((offset & 3) == 3) {
//                ArgumentException("Update spans the word, not supported")
//                        .throws(__trace("core.native.Unsafe"));
//            }
//            glong wordOffset = offset & ~3;
//            gint shift = (gint) (offset & 3) << 3;
//            if (BIG_ENDIAN) {
//                shift = 16 - shift;
//            }
//            gint mask = 0xFFFF << shift;
//            gint maskedExpected = (expected & 0xFFFF) << shift;
//            gint maskedX = (x & 0xFFFF) << shift;
//            gint fullWord;
//            do {
//                fullWord = getIntVolatile(o, wordOffset);
//                if ((fullWord & mask) != maskedExpected) {
//                    return (short) ((fullWord & mask) >> shift);
//                }
//            } while (!weakCompareAndSetInt(o, wordOffset,
//                                           fullWord, (fullWord & ~mask) | maskedX));
//            return expected;
        }

        gshort Unsafe::compareAndExchangeShortAcquire(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, expected, x);
        }

        gshort Unsafe::compareAndExchangeShortRelease(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, expected, x);
        }

        gshort Unsafe::compareAndExchangeShortRelaxed(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, expected, x);
        }

        gshort Unsafe::weakCompareAndExchangeShort(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, expected, x);
        }

        gshort Unsafe::weakCompareAndExchangeShortAcquire(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShort(o, offset, expected, x);
        }

        gshort Unsafe::weakCompareAndExchangeShortRelease(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShort(o, offset, expected, x);
        }

        gshort Unsafe::weakCompareAndExchangeShortRelaxed(core::Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShort(o, offset, expected, x);
        }

        gint Unsafe::getAndAddInt(Object &o, glong offset, gint delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 5);
#endif
            return InterlockedExchangeAdd((LONG volatile *) UnsafeImpl::getNativeAddress(o, offset), delta);
        }

        glong Unsafe::getAndAddLong(Object &o, glong offset, glong delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 5);
#endif
            return InterlockedExchangeAdd64((LONG64 volatile *) UnsafeImpl::getNativeAddress(o, offset), delta);
        }

        gint Unsafe::getAndSetInt(Object &o, glong offset, gint newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 5);
#endif
            return InterlockedExchange((LONG volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue);
        }

        glong Unsafe::getAndSetLong(Object &o, glong offset, glong newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 5);
#endif
            return InterlockedExchange64((LONG64 volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue);
        }

        gint Unsafe::getAndBitwiseOrInt(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            return InterlockedOr((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), mask);
        }

        glong Unsafe::getAndBitwiseOrLong(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            return InterlockedOr64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), mask);
        }

        gint Unsafe::getAndBitwiseAndInt(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            return InterlockedAnd((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), mask);
        }

        glong Unsafe::getAndBitwiseAndLong(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            return InterlockedAnd64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), mask);
        }

        gint Unsafe::getAndBitwiseXorInt(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            return InterlockedXor((volatile LONG *) UnsafeImpl::getNativeAddress(o, offset), mask);
        }

        glong Unsafe::getAndBitwiseXorLong(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            return InterlockedXor64((volatile LONG64 *) UnsafeImpl::getNativeAddress(o, offset), mask);
        }


    }
}
