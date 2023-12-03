//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/ArgumentException.h>
#include <core/Character.h>
#include <core/Byte.h>
#include <core/Short.h>
#include <core/Integer.h>
#include <core/Long.h>
#include <core/Float.h>
#include <core/Double.h>
#include <core/primitive/BooleanArray.h>
#include <core/primitive/ByteArray.h>
#include <core/primitive/CharArray.h>
#include <core/primitive/ShortArray.h>
#include <core/primitive/IntArray.h>
#include <core/primitive/LongArray.h>
#include <core/primitive/FloatArray.h>
#include <core/primitive/DoubleArray.h>
#include "Unsafe.h"

namespace core {

    namespace native {

        const gint Unsafe::ADDRESS_SIZE = CORE_ADDRESS_SIZE;
        const gbool Unsafe::BIG_ENDIAN = CORE_BYTE_ORDER == CORE_BIG_ENDIAN;
        const gint Unsafe::ARRAY_BOOLEAN_BASE_OFFSET = 16;
        const gint Unsafe::ARRAY_BYTE_BASE_OFFSET = ARRAY_BOOLEAN_BASE_OFFSET;
        const gint Unsafe::ARRAY_CHAR_BASE_OFFSET = 16;
        const gint Unsafe::ARRAY_SHORT_BASE_OFFSET = ARRAY_CHAR_BASE_OFFSET;
        const gint Unsafe::ARRAY_INT_BASE_OFFSET = 16;
        const gint Unsafe::ARRAY_LONG_BASE_OFFSET = 16;
        const gint Unsafe::ARRAY_FLOAT_BASE_OFFSET = ARRAY_INT_BASE_OFFSET;
        const gint Unsafe::ARRAY_DOUBLE_BASE_OFFSET = ARRAY_LONG_BASE_OFFSET;
        const gint Unsafe::ARRAY_REFERENCE_BASE_OFFSET =
                ADDRESS_SIZE == 4 ? ARRAY_INT_BASE_OFFSET : ARRAY_LONG_BASE_OFFSET;

        const gint Unsafe::ARRAY_BOOLEAN_INDEX_SCALE = 1;
        const gint Unsafe::ARRAY_BYTE_INDEX_SCALE = ARRAY_BOOLEAN_INDEX_SCALE;
        const gint Unsafe::ARRAY_CHAR_INDEX_SCALE = 2;
        const gint Unsafe::ARRAY_SHORT_INDEX_SCALE = ARRAY_CHAR_INDEX_SCALE;
        const gint Unsafe::ARRAY_INT_INDEX_SCALE = 4;
        const gint Unsafe::ARRAY_LONG_INDEX_SCALE = 8;
        const gint Unsafe::ARRAY_FLOAT_INDEX_SCALE = ARRAY_INT_INDEX_SCALE;
        const gint Unsafe::ARRAY_DOUBLE_INDEX_SCALE = ARRAY_LONG_INDEX_SCALE;
        const gint Unsafe::ARRAY_REFERENCE_INDEX_SCALE =
                ADDRESS_SIZE == 4 ? ARRAY_INT_INDEX_SCALE : ARRAY_LONG_INDEX_SCALE;

        Unsafe Unsafe::U = {};

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

            static gbool checkOffset(const Object &o, gint offset) {
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
                if (null == o)
                    return checkNativeAddress(offset);
                else
                    return checkOffset(o, offset);
            }

            static glong getNativeAddress(const Object &o, glong offset) {
                return (glong) &o + offset;
            }

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

        gint Unsafe::getInt(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gint *) address;
        }

        void Unsafe::putInt(Object &o, glong offset, gint x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gint *) address = x;
        }

        gint Unsafe::getInt(glong address) {
            return getInt(null, address);
        }

        void Unsafe::putInt(glong address, gint x) {
            return putInt(null, address, x);
        }

        gbool Unsafe::getBoolean(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gbool *) address;
        }

        void Unsafe::putBoolean(Object &o, glong offset, gbool x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gbool *) address = x;
        }

        gbool Unsafe::getBoolean(glong address) {
            return getBoolean(null, address);
        }

        void Unsafe::putBoolean(glong address, gbool x) {
            return putBoolean(null, address, x);
        }

        gbyte Unsafe::getByte(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gbyte *) address;
        }

        void Unsafe::putByte(Object &o, glong offset, gbyte x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gbyte *) address = x;
        }

        gbyte Unsafe::getByte(glong address) {
            return getByte(null, address);
        }

        void Unsafe::putByte(glong address, gbyte x) {
            return putByte(null, address, x);
        }

        gshort Unsafe::getShort(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gshort *) address;
        }

        void Unsafe::putShort(Object &o, glong offset, gshort x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gshort *) address = x;
        }

        gshort Unsafe::getShort(glong address) {
            return getShort(null, address);
        }

        void Unsafe::putShort(glong address, gshort x) {
            return putShort(null, address, x);
        }

        gchar Unsafe::getChar(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gchar *) address;
        }

        void Unsafe::putChar(Object &o, glong offset, gchar x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gchar *) address = x;
        }

        gchar Unsafe::getChar(glong address) {
            return getChar(null, address);
        }

        void Unsafe::putChar(glong address, gchar x) {
            return putChar(null, address, x);
        }

        glong Unsafe::getLong(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(glong *) address;
        }

        void Unsafe::putLong(Object &o, glong offset, glong x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(glong *) address = x;
        }

        glong Unsafe::getLong(glong address) {
            return getLong(null, address);
        }

        void Unsafe::putLong(glong address, glong x) {
            return putLong(null, address, x);
        }

        gfloat Unsafe::getFloat(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gfloat *) address;
        }

        void Unsafe::putFloat(Object &o, glong offset, gfloat x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gfloat *) address = x;
        }

        gfloat Unsafe::getFloat(glong address) {
            return getFloat(null, address);
        }

        void Unsafe::putFloat(glong address, gfloat x) {
            return putFloat(null, address, x);
        }

        gdouble Unsafe::getDouble(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            return *(gdouble *) address;
        }

        void Unsafe::putDouble(Object &o, glong offset, gdouble x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));
            *(gdouble *) address = x;
        }

        gdouble Unsafe::getDouble(glong address) {
            return getDouble(null, address);
        }

        void Unsafe::putDouble(glong address, gdouble x) {
            return putDouble(null, address, x);
        }

        Object &Unsafe::getReference(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));

            glong address2 = getAddress(address);
            return address2 == 0 ? null : *(Object *) address2;
        }

        void Unsafe::putReference(Object &o, glong offset, Object &x) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = null == o ? 0 : UnsafeImpl::getNativeAddress(o, offset);
            if (address == 0)
                MemoryError("Couldn't access to address 0x0").throws(__trace("core.native.Unsafe"));

            putAddress(address, UnsafeImpl::getNativeAddress(x, 0));
        }

        glong Unsafe::getAddress(const Object &o, glong offset) {
            if (!UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (ADDRESS_SIZE == 4)
                return (glong) getInt(o, offset);
            else
                return getLong(o, offset);
        }

        void Unsafe::putAddress(Object &o, glong offset, glong x) {
            if (!UnsafeImpl::checkPointer(o, offset) || !UnsafeImpl::checkNativeAddress(x))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (ADDRESS_SIZE == 4)
                putInt(o, offset, (gint) x);
            else
                putLong(o, offset, x);
        }

        glong Unsafe::getAddress(glong address) {
            return getAddress(null, address);
        }

        void Unsafe::putAddress(glong address, glong x) {
            putAddress(null, address, x);
        }

        glong Unsafe::allocateMemory(glong sizeInBytes) {
            glong heapSize = UnsafeImpl::alignToHeapWordSize(sizeInBytes);
            if (!UnsafeImpl::checkSize(heapSize))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (heapSize == 0)
                return 0;
            glong address = U.allocateMemoryImpl(heapSize);
            if (address == 0)
                MemoryError("Unable to allocate " + String::valueOf(heapSize) + " bytes")
                        .throws(__trace("core.native.Unsafe"));
            return address;
        }

        glong Unsafe::reallocateMemory(glong address, glong sizeInBytes) {
            glong heapSize = UnsafeImpl::alignToHeapWordSize(sizeInBytes);
            if (!UnsafeImpl::checkSize(heapSize) || !UnsafeImpl::checkPointer(null, address))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (heapSize == 0) {
                freeMemory(address);
                return 0;
            }
            glong address2 = address == 0 ? U.allocateMemoryImpl(heapSize) : U.reallocateMemoryImpl(address, heapSize);
            if (address2 == 0)
                MemoryError("Unable to allocate " + String::valueOf(heapSize) + " bytes")
                        .throws(__trace("core.native.Unsafe"));
            return address2;
        }

        void Unsafe::setMemory(Object &o, glong offset, glong sizeInBytes, gbyte value) {
            if (!UnsafeImpl::checkSize(sizeInBytes) || !UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));

            if (sizeInBytes == 0)
                return;
            setMemoryImpl(UnsafeImpl::getNativeAddress(o, offset), sizeInBytes, value);
        }

        void Unsafe::setMemory(glong address, glong sizeInBytes, gbyte value) {
            setMemory(null, address, sizeInBytes, value);
        }

        void Unsafe::copyMemory(const Object &src, glong srcOffset, Object &dest, glong destOffset,
                                glong sizeInBytes) {
            if (!UnsafeImpl::checkSize(sizeInBytes) ||
                !UnsafeImpl::checkPointer(src, srcOffset) ||
                !UnsafeImpl::checkPointer(dest, destOffset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));

            if (sizeInBytes == 0)
                return;
            copyMemory(src, srcOffset, dest, destOffset, sizeInBytes);
        }

        void Unsafe::copyMemory(glong srcAddress, glong destAddress, glong sizeInBytes) {
            copyMemory(null, srcAddress, null, destAddress, sizeInBytes);
        }

        void Unsafe::copySwapMemory(Object &src, glong srcOffset, Object &dest, glong destOffset, glong sizeInBytes,
                                    glong elemSize) {
            if (!UnsafeImpl::checkSize(sizeInBytes) ||
                !UnsafeImpl::checkPointer(src, srcOffset) ||
                !UnsafeImpl::checkPointer(dest, destOffset) ||
                (elemSize != 2 && elemSize != 4 && elemSize != 8 && sizeInBytes % elemSize != 0))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));

            if (sizeInBytes == 0)
                return;
            copySwapMemoryImpl(UnsafeImpl::getNativeAddress(src, srcOffset),
                               UnsafeImpl::getNativeAddress(dest, destOffset), sizeInBytes, elemSize);
        }

        void Unsafe::copySwapMemory(glong srcAddress, glong destAddress, glong sizeInBytes, glong elemSize) {
            copySwapMemory(null, srcAddress, null, destAddress, sizeInBytes, elemSize);
        }

        void Unsafe::freeMemory(glong address) {
            if (!UnsafeImpl::checkPointer(null, address))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (address == 0)
                return;
            freeMemoryImpl(address);
        }

        namespace {
            interface Heap {
                glong address = 0;
                Heap *next = null;
            };

            interface Cache {
                Heap *head = null;
                Heap *tail = null;

                void add(glong address) {
                    tail == null ? head = tail = new Heap(address, null) : tail = tail->next = new Heap(address, null);
                }

                gbool exists(glong address) const {
                    Heap *heap = head;
                    while (heap != null)
                        if (heap->address == address)
                            return true;
                        else
                            heap = heap->next;
                    return false;
                }

                void remove(glong address) {
                    Heap *heap = head;
                    Heap *heap1 = null;
                    while (heap != null) {
                        if (heap->address == address) {
                            if (heap1 == null) {
                                head = heap->next;
                                if (head == null)
                                    tail = null;
                            } else {
                                if (heap == tail)
                                    tail = heap1;
                                heap1->next = heap->next;
                            }
                            heap->address = 0;
                            heap->next = null;
                            delete heap;
                            return;
                        }
                        heap1 = heap;
                        heap = heap->next;
                    }
                }
            };

            Cache cache;
        }

        gbool Unsafe::loadInstance(glong address) {
            return cache.exists(address);
        }

        void Unsafe::storeInstance(glong address) {
            cache.add(address);
        }

        void Unsafe::deleteInstance(glong address) {
            cache.remove(address);
        }

        Unsafe::~Unsafe() {
            // clean all memory cache
            while (cache.head != null) {
                Heap *heap = cache.head;
                cache.head = heap->next;
                heap->next = null;
                glong addr = heap->address;
                heap->address = 0;
                delete heap;
                freeMemory(addr);
                heap = null;
            }
        }

        namespace {

            // Zero-extend an integer
            gint toUnsignedInt(gbyte n) { return n & 0xff; }

            gint toUnsignedInt(gshort n) { return n & 0xffff; }

            glong toUnsignedLong(gbyte n) { return n & 0xffl; }

            glong toUnsignedLong(gshort n) { return n & 0xffffl; }

            glong toUnsignedLong(gint n) { return n & 0xffffffffl; }

            gint pickPos(gint top, gint pos) { return Unsafe::BIG_ENDIAN ? top - pos : pos; }

            // These methods construct integers from bytes.  The gbyte ordering
            // is the native endianness of this platform.
            glong makeLong(gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5, gbyte i6, gbyte i7) {
                return ((toUnsignedLong(i0) << pickPos(56, 0))
                        | (toUnsignedLong(i1) << pickPos(56, 8))
                        | (toUnsignedLong(i2) << pickPos(56, 16))
                        | (toUnsignedLong(i3) << pickPos(56, 24))
                        | (toUnsignedLong(i4) << pickPos(56, 32))
                        | (toUnsignedLong(i5) << pickPos(56, 40))
                        | (toUnsignedLong(i6) << pickPos(56, 48))
                        | (toUnsignedLong(i7) << pickPos(56, 56)));
            }

            glong makeLong(gshort i0, gshort i1, gshort i2, gshort i3) {
                return ((toUnsignedLong(i0) << pickPos(48, 0))
                        | (toUnsignedLong(i1) << pickPos(48, 16))
                        | (toUnsignedLong(i2) << pickPos(48, 32))
                        | (toUnsignedLong(i3) << pickPos(48, 48)));
            }

            glong makeLong(gint i0, gint i1) {
                return (toUnsignedLong(i0) << pickPos(32, 0))
                       | (toUnsignedLong(i1) << pickPos(32, 32));
            }

            gint makeInt(gshort i0, gshort i1) {
                return (toUnsignedInt(i0) << pickPos(16, 0))
                       | (toUnsignedInt(i1) << pickPos(16, 16));
            }

            gint makeInt(gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
                return ((toUnsignedInt(i0) << pickPos(24, 0))
                        | (toUnsignedInt(i1) << pickPos(24, 8))
                        | (toUnsignedInt(i2) << pickPos(24, 16))
                        | (toUnsignedInt(i3) << pickPos(24, 24)));
            }

            gshort makeShort(gbyte i0, gbyte i1) {
                return (gshort) ((toUnsignedInt(i0) << pickPos(8, 0))
                                 | (toUnsignedInt(i1) << pickPos(8, 8)));
            }

            gbyte pick(gbyte le, gbyte be) { return Unsafe::BIG_ENDIAN ? be : le; }

            gshort pick(gshort le, gshort be) { return Unsafe::BIG_ENDIAN ? be : le; }

            gint pick(gint le, gint be) { return Unsafe::BIG_ENDIAN ? be : le; }

            // These methods write integers to memory from smaller parts
            // provided by their caller.  The ordering in which these parts
            // are written is the native endianness of this platform.
            void
            putLongParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5, gbyte i6,
                         gbyte i7) {
                Unsafe::U.putByte(o, offset + 0, pick(i0, i7));
                Unsafe::U.putByte(o, offset + 1, pick(i1, i6));
                Unsafe::U.putByte(o, offset + 2, pick(i2, i5));
                Unsafe::U.putByte(o, offset + 3, pick(i3, i4));
                Unsafe::U.putByte(o, offset + 4, pick(i4, i3));
                Unsafe::U.putByte(o, offset + 5, pick(i5, i2));
                Unsafe::U.putByte(o, offset + 6, pick(i6, i1));
                Unsafe::U.putByte(o, offset + 7, pick(i7, i0));
            }

            void putLongParts(Object &o, glong offset, gshort i0, gshort i1, gshort i2, gshort i3) {
                Unsafe::U.putShort(o, offset + 0, pick(i0, i3));
                Unsafe::U.putShort(o, offset + 2, pick(i1, i2));
                Unsafe::U.putShort(o, offset + 4, pick(i2, i1));
                Unsafe::U.putShort(o, offset + 6, pick(i3, i0));
            }

            void putLongParts(Object &o, glong offset, gint i0, gint i1) {
                Unsafe::U.putInt(o, offset + 0, pick(i0, i1));
                Unsafe::U.putInt(o, offset + 4, pick(i1, i0));
            }

            void putIntParts(Object &o, glong offset, gshort i0, gshort i1) {
                Unsafe::U.putShort(o, offset + 0, pick(i0, i1));
                Unsafe::U.putShort(o, offset + 2, pick(i1, i0));
            }

            void putIntParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
                Unsafe::U.putByte(o, offset + 0, pick(i0, i3));
                Unsafe::U.putByte(o, offset + 1, pick(i1, i2));
                Unsafe::U.putByte(o, offset + 2, pick(i2, i1));
                Unsafe::U.putByte(o, offset + 3, pick(i3, i0));
            }

            void putShortParts(Object &o, glong offset, gbyte i0, gbyte i1) {
                Unsafe::U.putByte(o, offset + 0, pick(i0, i1));
                Unsafe::U.putByte(o, offset + 1, pick(i1, i0));
            }

            // Maybe gbyte-reverse an integer
            gchar convEndian(gbool big, gchar n) { return big == Unsafe::BIG_ENDIAN ? n : Character::reverseBytes(n); }

            gshort convEndian(gbool big, gshort n) { return (gshort) convEndian(big, (gchar) n); }

            gint convEndian(gbool big, gint n) { return big == Unsafe::BIG_ENDIAN ? n : Integer::reverseBytes(n); }

            glong convEndian(gbool big, glong n) { return big == Unsafe::BIG_ENDIAN ? n : Long::reverseBytes(n); }
        }

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
            putShortUnaligned(o, offset, (gshort) x);
        }

        void Unsafe::putCharUnaligned(Object &o, glong offset, gchar x, gbool bigEndian) {
            putCharUnaligned(o, offset, convEndian(bigEndian, x));
        }

        gbool Unsafe::compareAndSetReference(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetReference(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetReferenceAcquire(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetReferenceRelaxed(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetReferenceRelease(Object &o, glong offset, const Object &expected, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeReference(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBoolean(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeBoolean(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeBooleanAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeBooleanRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeBooleanRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBoolean(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeBoolean(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeBooleanAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeBooleanRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeBooleanRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByte(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByteAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByteRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByteRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByte(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByteAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByteRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetByteRelaxed(Object &o, glong offset, gbyte expected, gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByteRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShort(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShortAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShortRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetShortRelaxed(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShortRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShort(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShort(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShortAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShortRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetShortRelaxed(Object &o, glong offset, gshort expected, gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeShortRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetChar(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeChar(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeCharAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeCharRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeCharRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetChar(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeChar(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeCharAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeCharRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeCharRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetInt(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeInt(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeIntAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetIntRelease(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeIntRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetIntRelaxed(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeIntRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetInt(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeInt(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeIntAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetIntRelease(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeIntRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetIntRelaxed(Object &o, glong offset, gint expected, gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeIntRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLong(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeLong(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeLongAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLongRelease(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeLongRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetLongRelaxed(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeLongRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLong(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeLong(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeLongAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLongRelease(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeLongRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetLongRelaxed(Object &o, glong offset, glong expected, glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeLongRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeFloat(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeFloatAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeFloatRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeFloatRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeFloat(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeFloatAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeFloatRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeFloatRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeDouble(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeDoubleAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeDoubleRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::compareAndSetDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeDoubleRelaxed(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeDouble(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeDoubleAcquire(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeDoubleRelease(o, offset, expected, x) == x;
        }

        gbool Unsafe::weakCompareAndSetDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeDoubleRelaxed(o, offset, expected, x) == x;
        }

        gfloat Unsafe::compareAndExchangeFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        compareAndExchangeInt(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::compareAndExchangeFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        compareAndExchangeIntAcquire(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::compareAndExchangeFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        compareAndExchangeIntRelease(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::compareAndExchangeFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        compareAndExchangeIntRelaxed(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::weakCompareAndExchangeFloat(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        weakCompareAndExchangeInt(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::weakCompareAndExchangeFloatAcquire(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        weakCompareAndExchangeIntAcquire(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::weakCompareAndExchangeFloatRelease(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        weakCompareAndExchangeIntRelease(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gfloat Unsafe::weakCompareAndExchangeFloatRelaxed(Object &o, glong offset, gfloat expected, gfloat x) {
            try {
                return Float::fromIntBits(
                        weakCompareAndExchangeIntRelaxed(o, offset, Float::toIntBits(expected), Float::toIntBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::compareAndExchangeDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        compareAndExchangeLong(o, offset, Double::toLongBits(expected), Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::compareAndExchangeDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        compareAndExchangeLongAcquire(o, offset, Double::toLongBits(expected), Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::compareAndExchangeDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        compareAndExchangeLongRelease(o, offset, Double::toLongBits(expected), Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::compareAndExchangeDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        compareAndExchangeLongRelaxed(o, offset, Double::toLongBits(expected), Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::weakCompareAndExchangeDouble(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        weakCompareAndExchangeLong(o, offset, Double::toLongBits(expected), Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::weakCompareAndExchangeDoubleAcquire(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        weakCompareAndExchangeLongAcquire(o, offset, Double::toLongBits(expected),
                                                          Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::weakCompareAndExchangeDoubleRelease(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        weakCompareAndExchangeLongRelease(o, offset, Double::toLongBits(expected),
                                                          Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gdouble Unsafe::weakCompareAndExchangeDoubleRelaxed(Object &o, glong offset, gdouble expected, gdouble x) {
            try {
                return Double::fromLongBits(
                        weakCompareAndExchangeLongRelaxed(o, offset, Double::toLongBits(expected),
                                                          Double::toLongBits(x)));
            } catch (const Throwable &thr) {
                thr.throws(__trace("Core::Unsafe"));
            }
        }

        gbool Unsafe::compareAndExchangeBoolean(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByte(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::compareAndExchangeBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByteAcquire(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::compareAndExchangeBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByteRelease(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::compareAndExchangeBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeByteRelaxed(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::weakCompareAndExchangeBoolean(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByte(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::weakCompareAndExchangeBooleanAcquire(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByteAcquire(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::weakCompareAndExchangeBooleanRelease(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByteRelease(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gbool Unsafe::weakCompareAndExchangeBooleanRelaxed(Object &o, glong offset, gbool expected, gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return weakCompareAndExchangeByteRelaxed(o, offset, expected ? 1 : 0, x ? 1 : 0);
        }

        gchar Unsafe::compareAndExchangeChar(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::compareAndExchangeCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::compareAndExchangeCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::compareAndExchangeCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::weakCompareAndExchangeChar(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShort(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::weakCompareAndExchangeCharAcquire(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShortAcquire(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::weakCompareAndExchangeCharRelease(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShortRelease(o, offset, (gshort) expected, (gshort) x);
        }

        gchar Unsafe::weakCompareAndExchangeCharRelaxed(Object &o, glong offset, gchar expected, gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return compareAndExchangeShortRelaxed(o, offset, (gshort) expected, (gshort) x);
        }

        Object volatile &Unsafe::getReferenceVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ? getIntVolatile(o, offset) : getLongVolatile(o, offset);
            return address == 0 ? null : *(Object *) address;
        }

        void Unsafe::putReferenceVolatile(Object &o, glong offset, Object volatile &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (ADDRESS_SIZE == 4)
                putIntVolatile(o, offset, (gint) UnsafeImpl::getNativeAddress((Object &) x, 0));
            else
                putLongVolatile(o, offset, UnsafeImpl::getNativeAddress((Object &) x, 0));
        }

        gint volatile Unsafe::getIntVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), 4);
#endif
            return (gint volatile) getInt(o, offset);
        }

        void Unsafe::putIntVolatile(Object &o, glong offset, volatile gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 4);
            return;
#endif
            putInt(o, offset, (gint) x);
        }

        glong volatile Unsafe::getLongVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), 4);
#endif
            return (glong volatile) getLong(o, offset);
        }

        void Unsafe::putLongVolatile(Object &o, glong offset, volatile glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 4);
            return;
#endif
            putLong(o, offset, (glong) x);
        }

        gshort volatile Unsafe::getShortVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), 4);
#endif
            return (gshort volatile) getShort(o, offset);
        }

        void Unsafe::putShortVolatile(Object &o, glong offset, volatile gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 4);
            return;
#endif
            putInt(o, offset, (gshort) x);
        }

        gbyte volatile Unsafe::getByteVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), 4);
#endif
            return (gbyte volatile) getByte(o, offset);
        }

        void Unsafe::putByteVolatile(Object &o, glong offset, volatile gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 4);
            return;
#endif
            putInt(o, offset, (gbyte) x);
        }

        gbool volatile Unsafe::getBooleanVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getByteVolatile(o, offset) != 0;
        }

        void Unsafe::putBooleanVolatile(Object &o, glong offset, volatile gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putByteVolatile(o, offset, x);
        }

        gchar volatile Unsafe::getCharVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar volatile) getShortVolatile(o, offset);
        }

        void Unsafe::putCharVolatile(Object &o, glong offset, volatile gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putShortVolatile(o, offset, (gshort) x);
        }

        gfloat volatile Unsafe::getFloatVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gfloat volatile) Float::fromIntBits(getIntVolatile(o, offset));
        }

        void Unsafe::putFloatVolatile(Object &o, glong offset, volatile gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putIntVolatile(o, offset, Float::toIntBits(x));
        }

        gdouble volatile Unsafe::getDoubleVolatile(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gdouble volatile) Double::fromLongBits(getLongVolatile(o, offset));
        }

        void Unsafe::putDoubleVolatile(Object &o, glong offset, volatile gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putLongVolatile(o, offset, Double::toLongBits(x));
        }

        Object &Unsafe::getReferenceAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ? getIntAcquire(o, offset) : getLongAcquire(o, offset);
            return address == 0 ? null : *(Object *) address;
        }

        gint Unsafe::getIntAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), 2);
#endif
            return (gint volatile) getInt(o, offset);
        }

        glong Unsafe::getLongAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), 2);
#endif
            return (glong volatile) getLong(o, offset);
        }

        gshort Unsafe::getShortAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), 2);
#endif
            return (gshort volatile) getShort(o, offset);
        }

        gbyte Unsafe::getByteAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), 2);
#endif
            return (gbyte volatile) getByte(o, offset);
        }

        gbool Unsafe::getBooleanAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getByteAcquire(o, offset) != 0;
        }

        gchar Unsafe::getCharAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar volatile) getShortAcquire(o, offset);
        }

        gfloat Unsafe::getFloatAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gfloat volatile) Float::fromIntBits(getIntAcquire(o, offset));
        }

        gdouble Unsafe::getDoubleAcquire(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gdouble volatile) Double::fromLongBits(getLongAcquire(o, offset));
        }

        void Unsafe::putReferenceRelease(Object &o, glong offset, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (ADDRESS_SIZE == 4)
                putIntRelease(o, offset, (gint) UnsafeImpl::getNativeAddress((Object &) x, 0));
            else
                putLongRelease(o, offset, UnsafeImpl::getNativeAddress((Object &) x, 0));
        }

        void Unsafe::putIntRelease(Object &o, glong offset, volatile gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 3);
            return;
#endif
            putInt(o, offset, (gint) x);
        }

        void Unsafe::putLongRelease(Object &o, glong offset, volatile glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 3);
            return;
#endif
            putLong(o, offset, (glong) x);
        }

        void Unsafe::putShortRelease(Object &o, glong offset, volatile gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 3);
            return;
#endif
            putInt(o, offset, (gshort) x);
        }

        void Unsafe::putByteRelease(Object &o, glong offset, volatile gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 3);
            return;
#endif
            putInt(o, offset, (gbyte) x);
        }

        void Unsafe::putBooleanRelease(Object &o, glong offset, volatile gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putByteRelease(o, offset, x);
        }

        void Unsafe::putCharRelease(Object &o, glong offset, volatile gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putShortRelease(o, offset, (gshort) x);
        }

        void Unsafe::putFloatRelease(Object &o, glong offset, volatile gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putIntRelease(o, offset, Float::toIntBits(x));
        }

        void Unsafe::putDoubleRelease(Object &o, glong offset, volatile gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putLongRelease(o, offset, Double::toLongBits(x));
        }

        Object &Unsafe::getReferenceRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ? getIntRelaxed(o, offset) : getLongRelaxed(o, offset);
            return address == 0 ? null : *(Object *) address;
        }

        void Unsafe::putReferenceRelaxed(Object &o, glong offset, Object &x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            if (ADDRESS_SIZE == 4)
                putIntRelaxed(o, offset, (gint) UnsafeImpl::getNativeAddress((Object &) x, 0));
            else
                putLongRelaxed(o, offset, UnsafeImpl::getNativeAddress((Object &) x, 0));
        }

        gint Unsafe::getIntRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), 0);
#endif
            return (gint volatile) getInt(o, offset);
        }

        void Unsafe::putIntRelaxed(Object &o, glong offset, volatile gint x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 0);
            return;
#endif
            putInt(o, offset, (gint) x);
        }

        glong Unsafe::getLongRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), 0);
#endif
            return (glong volatile) getLong(o, offset);
        }

        void Unsafe::putLongRelaxed(Object &o, glong offset, volatile glong x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 0);
            return;
#endif
            putLong(o, offset, (glong) x);
        }

        gshort Unsafe::getShortRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), 0);
#endif
            return (gshort volatile) getShort(o, offset);
        }

        void Unsafe::putShortRelaxed(Object &o, glong offset, volatile gshort x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 0);
            return;
#endif
            putInt(o, offset, (gshort) x);
        }

        gbyte Unsafe::getByteRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            return __atomic_load_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), 0);
#endif
            return (gbyte volatile) getByte(o, offset);
        }

        void Unsafe::putByteRelaxed(Object &o, glong offset, volatile gbyte x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_load_n)
            __atomic_store_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), x, 0);
            return;
#endif
            putInt(o, offset, (gbyte) x);
        }

        gbool Unsafe::getBooleanRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getByteRelaxed(o, offset) != 0;
        }

        void Unsafe::putBooleanRelaxed(Object &o, glong offset, volatile gbool x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putByteRelaxed(o, offset, x);
        }

        gchar Unsafe::getCharRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar volatile) getShortRelaxed(o, offset);
        }

        void Unsafe::putCharRelaxed(Object &o, glong offset, volatile gchar x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putShortRelaxed(o, offset, (gshort) x);
        }

        gfloat Unsafe::getFloatRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gfloat volatile) Float::fromIntBits(getIntRelaxed(o, offset));
        }

        void Unsafe::putFloatRelaxed(Object &o, glong offset, volatile gfloat x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putIntRelaxed(o, offset, Float::toIntBits(x));
        }

        gdouble Unsafe::getDoubleRelaxed(const Object &o, glong offset) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gdouble volatile) Double::fromLongBits(getLongRelaxed(o, offset));
        }

        void Unsafe::putDoubleRelaxed(Object &o, glong offset, volatile gdouble x) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            putLongRelaxed(o, offset, Double::toLongBits(x));
        }

        gint Unsafe::getAndAddIntAcquire(Object &o, glong offset, gint delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 2);
#endif
            return getAndAddInt(o, offset, delta);
        }

        gint Unsafe::getAndAddIntRelease(Object &o, glong offset, gint delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 3);
#endif
            return getAndAddInt(o, offset, delta);
        }

        gint Unsafe::getAndAddIntRelaxed(Object &o, glong offset, gint delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 3);
#endif
            return getAndAddInt(o, offset, delta);
        }

        glong Unsafe::getAndAddLongAcquire(Object &o, glong offset, glong delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 2);
#endif
            return getAndAddLong(o, offset, delta);
        }

        glong Unsafe::getAndAddLongRelease(Object &o, glong offset, glong delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 3);
#endif
            return getAndAddLong(o, offset, delta);
        }

        glong Unsafe::getAndAddLongRelaxed(Object &o, glong offset, glong delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 3);
#endif
            return getAndAddLong(o, offset, delta);
        }

        gbyte Unsafe::getAndAddByte(Object &o, glong offset, gbyte delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 4);
#endif
            gbyte v;
            do {
                v = getByte(o, offset);
            } while (!weakCompareAndSetByteRelease(o, offset, v, (gbyte) (v + delta)));
            return v;
        }

        gbyte Unsafe::getAndAddByteAcquire(Object &o, glong offset, gbyte delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 2);
#endif
            gbyte v;
            do {
                v = getByteAcquire(o, offset);
            } while (!weakCompareAndSetByteAcquire(o, offset, v, (gbyte) (v + delta)));
            return v;
        }

        gbyte Unsafe::getAndAddByteRelease(Object &o, glong offset, gbyte delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 3);
#endif
            gbyte v;
            do {
                v = getByte(o, offset);
            } while (!weakCompareAndSetByteRelease(o, offset, v, (gbyte) (v + delta)));
            return v;
        }

        gbyte Unsafe::getAndAddByteRelaxed(Object &o, glong offset, gbyte delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 0);
#endif
            gbyte v;
            do {
                v = getByteRelaxed(o, offset);
            } while (!weakCompareAndSetByteRelaxed(o, offset, v, (gbyte) (v + delta)));
            return v;
        }

        gshort Unsafe::getAndAddShort(Object &o, glong offset, gshort delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 4);
#endif
            gshort v;
            do {
                v = getShort(o, offset);
            } while (!weakCompareAndSetShortRelease(o, offset, v, (gshort) (v + delta)));
            return v;
        }

        gshort Unsafe::getAndAddShortAcquire(Object &o, glong offset, gshort delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 2);
#endif
            gshort v;
            do {
                v = getShortAcquire(o, offset);
            } while (!weakCompareAndSetShortAcquire(o, offset, v, (gshort) (v + delta)));
            return v;
        }

        gshort Unsafe::getAndAddShortRelease(Object &o, glong offset, gshort delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 3);
#endif
            gshort v;
            do {
                v = getShort(o, offset);
            } while (!weakCompareAndSetShortRelease(o, offset, v, (gshort) (v + delta)));
            return v;
        }

        gshort Unsafe::getAndAddShortRelaxed(Object &o, glong offset, gshort delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_add)
            return __atomic_fetch_add((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), delta, 0);
#endif
            gshort v;
            do {
                v = getShortRelaxed(o, offset);
            } while (!weakCompareAndSetShortRelaxed(o, offset, v, (gshort) (v + delta)));
            return v;
        }

        gchar Unsafe::getAndAddChar(Object &o, glong offset, gchar delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndAddShort(o, offset, (gshort) delta);
        }

        gchar Unsafe::getAndAddCharAcquire(Object &o, glong offset, gchar delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndAddShortAcquire(o, offset, (gshort) delta);
        }

        gchar Unsafe::getAndAddCharRelease(Object &o, glong offset, gchar delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndAddShortRelease(o, offset, (gshort) delta);
        }

        gchar Unsafe::getAndAddCharRelaxed(Object &o, glong offset, gchar delta) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndAddShortRelaxed(o, offset, (gshort) delta);
        }

        gfloat Unsafe::getAndAddFloat(Object &o, glong offset, gfloat delta) {
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntVolatile(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetInt(o, offset, expectedBits, Float::toIntBits(v + delta)));
            return v;
        }

        gfloat Unsafe::getAndAddFloatAcquire(Object &o, glong offset, gfloat delta) {
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntAcquire(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetIntAcquire(o, offset, expectedBits, Float::toIntBits(v + delta)));
            return v;
        }

        gfloat Unsafe::getAndAddFloatRelease(Object &o, glong offset, gfloat delta) {
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetIntRelease(o, offset, expectedBits, Float::toIntBits(v + delta)));
            return v;
        }

        gfloat Unsafe::getAndAddFloatRelaxed(Object &o, glong offset, gfloat delta) {
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntRelaxed(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetIntRelaxed(o, offset, expectedBits, Float::toIntBits(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDouble(Object &o, glong offset, gdouble delta) {
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongVolatile(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLong(o, offset, expectedBits, Double::toLongBits(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDoubleAcquire(Object &o, glong offset, gdouble delta) {
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongAcquire(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLongAcquire(o, offset, expectedBits, Double::toLongBits(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDoubleRelease(Object &o, glong offset, gdouble delta) {
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLongRelease(o, offset, expectedBits, Double::toLongBits(v + delta)));
            return v;
        }

        gdouble Unsafe::getAndAddDoubleRelaxed(Object &o, glong offset, gdouble delta) {
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongRelaxed(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLongRelaxed(o, offset, expectedBits, Double::toLongBits(v + delta)));
            return v;
        }

        gint Unsafe::getAndSetIntAcquire(Object &o, glong offset, gint newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 2);
#endif
            return getAndSetInt(o, offset, newValue);
        }

        glong Unsafe::getAndSetLongAcquire(Object &o, glong offset, glong newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 2);
#endif
            return getAndSetLong(o, offset, newValue);
        }

        gint Unsafe::getAndSetIntRelease(Object &o, glong offset, gint newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 3);
#endif
            return getAndSetInt(o, offset, newValue);
        }

        glong Unsafe::getAndSetLongRelease(Object &o, glong offset, glong newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 3);
#endif
            return getAndSetLong(o, offset, newValue);
        }

        gint Unsafe::getAndSetIntRelaxed(Object &o, glong offset, gint newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gint volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 0);
#endif
            return getAndSetInt(o, offset, newValue);
        }

        glong Unsafe::getAndSetLongRelaxed(Object &o, glong offset, glong newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((glong volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 0);
#endif
            return getAndSetLong(o, offset, newValue);
        }

        gbyte Unsafe::getAndSetByte(Object &o, glong offset, gbyte newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 4);
#endif
            gbyte v;
            do {
                v = getByte(o, offset);
            } while (!weakCompareAndSetByteRelease(o, offset, v, (gbyte) newValue));
            return v;
        }

        gbyte Unsafe::getAndSetByteAcquire(Object &o, glong offset, gbyte newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 2);
#endif
            gbyte v;
            do {
                v = getByteAcquire(o, offset);
            } while (!weakCompareAndSetByteAcquire(o, offset, v, (gbyte) newValue));
            return v;
        }

        gbyte Unsafe::getAndSetByteRelease(Object &o, glong offset, gbyte newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 3);
#endif
            gbyte v;
            do {
                v = getByte(o, offset);
            } while (!weakCompareAndSetByteRelease(o, offset, v, (gbyte) newValue));
            return v;
        }

        gbyte Unsafe::getAndSetByteRelaxed(Object &o, glong offset, gbyte newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gbyte volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 0);
#endif
            gbyte v;
            do {
                v = getByteRelaxed(o, offset);
            } while (!weakCompareAndSetByteRelaxed(o, offset, v, (gbyte) newValue));
            return v;
        }

        gshort Unsafe::getAndSetShort(Object &o, glong offset, gshort newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 4);
#endif
            gshort v;
            do {
                v = getShort(o, offset);
            } while (!weakCompareAndSetShortRelease(o, offset, v, (gshort) newValue));
            return v;
        }

        gshort Unsafe::getAndSetShortAcquire(Object &o, glong offset, gshort newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 2);
#endif
            gshort v;
            do {
                v = getShortAcquire(o, offset);
            } while (!weakCompareAndSetShortAcquire(o, offset, v, (gshort) newValue));
            return v;
        }

        gshort Unsafe::getAndSetShortRelease(Object &o, glong offset, gshort newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 3);
#endif
            gshort v;
            do {
                v = getShort(o, offset);
            } while (!weakCompareAndSetShortRelease(o, offset, v, (gshort) newValue));
            return v;
        }

        gshort Unsafe::getAndSetShortRelaxed(Object &o, glong offset, gshort newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_exchange_n)
            return __atomic_exchange_n((gshort volatile *) UnsafeImpl::getNativeAddress(o, offset), newValue, 0);
#endif
            gshort v;
            do {
                v = getShortRelaxed(o, offset);
            } while (!weakCompareAndSetShortRelaxed(o, offset, v, (gshort) newValue));
            return v;
        }

        gchar Unsafe::getAndSetChar(Object &o, glong offset, gchar newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndSetShort(o, offset, (gshort) newValue);
        }

        gchar Unsafe::getAndSetCharAcquire(Object &o, glong offset, gchar newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndSetShortAcquire(o, offset, (gshort) newValue);
        }

        gchar Unsafe::getAndSetCharRelease(Object &o, glong offset, gchar newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndSetShortRelease(o, offset, (gshort) newValue);
        }

        gchar Unsafe::getAndSetCharRelaxed(Object &o, glong offset, gchar newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return (gchar) getAndSetShortRelaxed(o, offset, (gshort) newValue);
        }

        gbool Unsafe::getAndSetBoolean(Object &o, glong offset, gbool newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndSetByte(o, offset, newValue ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndSetBooleanAcquire(Object &o, glong offset, gbool newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndSetByteAcquire(o, offset, newValue ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndSetBooleanRelease(Object &o, glong offset, gbool newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndSetByteRelease(o, offset, newValue ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndSetBooleanRelaxed(Object &o, glong offset, gbool newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndSetByteRelaxed(o, offset, newValue ? 1 : 0) != 0;
        }

        gfloat Unsafe::getAndSetFloat(Object &o, glong offset, gfloat newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntVolatile(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetInt(o, offset, expectedBits, Float::toIntBits(newValue)));
            return v;
        }

        gfloat Unsafe::getAndSetFloatAcquire(Object &o, glong offset, gfloat newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntAcquire(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetIntAcquire(o, offset, expectedBits, Float::toIntBits(newValue)));
            return v;
        }

        gfloat Unsafe::getAndSetFloatRelease(Object &o, glong offset, gfloat newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getInt(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetIntRelease(o, offset, expectedBits, Float::toIntBits(newValue)));
            return v;
        }

        gfloat Unsafe::getAndSetFloatRelaxed(Object &o, glong offset, gfloat newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            gint expectedBits;
            float v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getIntRelaxed(o, offset);
                v = Float::fromIntBits(expectedBits);
            } while (!weakCompareAndSetIntRelaxed(o, offset, expectedBits, Float::toIntBits(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDouble(Object &o, glong offset, gdouble newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongVolatile(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLong(o, offset, expectedBits, Double::toLongBits(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDoubleAcquire(Object &o, glong offset, gdouble newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongAcquire(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLongAcquire(o, offset, expectedBits, Double::toLongBits(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDoubleRelease(Object &o, glong offset, gdouble newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLong(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLongRelease(o, offset, expectedBits, Double::toLongBits(newValue)));
            return v;
        }

        gdouble Unsafe::getAndSetDoubleRelaxed(Object &o, glong offset, gdouble newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong expectedBits;
            double v;
            do {
                // Load and CAS with the raw bits to avoid issues with NaNs and
                // possible bit conversion from signaling NaNs to quiet NaNs that
                // may result in the loop not terminating.
                expectedBits = getLongRelaxed(o, offset);
                v = Double::fromLongBits(expectedBits);
            } while (!weakCompareAndSetLongRelaxed(o, offset, expectedBits, Double::toLongBits(newValue)));
            return v;
        }

        Object &Unsafe::getAndSetReference(Object &o, glong offset, Object &newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            getAndSetInt(o, offset, (gint) UnsafeImpl::getNativeAddress(newValue, 0)) :
                            getAndSetLong(o, offset, UnsafeImpl::getNativeAddress(newValue, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::getAndSetReferenceAcquire(Object &o, glong offset, Object &newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            getAndSetIntAcquire(o, offset, (gint) UnsafeImpl::getNativeAddress(newValue, 0)) :
                            getAndSetLongAcquire(o, offset, UnsafeImpl::getNativeAddress(newValue, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::getAndSetReferenceRelease(Object &o, glong offset, Object &newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            getAndSetIntRelease(o, offset, (gint) UnsafeImpl::getNativeAddress(newValue, 0)) :
                            getAndSetLongRelease(o, offset, UnsafeImpl::getNativeAddress(newValue, 0));
            return address == 0 ? null : *(Object *) address;
        }

        Object &Unsafe::getAndSetReferenceRelaxed(Object &o, glong offset, Object &newValue) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            glong address = ADDRESS_SIZE == 4 ?
                            getAndSetIntRelaxed(o, offset, (gint) UnsafeImpl::getNativeAddress(newValue, 0)) :
                            getAndSetLongRelaxed(o, offset, UnsafeImpl::getNativeAddress(newValue, 0));
            return address == 0 ? null : *(Object *) address;
        }

        gbyte Unsafe::getAndBitwiseOrByte(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            gbyte current;
            do {
                current = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current | mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseOrByteAcquire(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            gbyte current;
            do {
                current = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current | mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseOrByteRelease(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            gbyte current;
            do {
                current = getByte(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current | mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseOrByteRelaxed(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            gbyte current;
            do {
                current = getByteRelaxed(o, offset);
            } while (!weakCompareAndSetByteRelaxed(o, offset, current, (gbyte) (current | mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseOrShort(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            gshort current;
            do {
                current = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current | mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseOrShortAcquire(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            gshort current;
            do {
                current = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current | mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseOrShortRelease(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            gshort current;
            do {
                current = getShort(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current | mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseOrShortRelaxed(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            gshort current;
            do {
                current = getShortRelaxed(o, offset);
            } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gshort) (current | mask)));
            return current;
        }

        gint Unsafe::getAndBitwiseOrIntAcquire(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            return getAndBitwiseAndInt(o, offset, mask);
        }

        gint Unsafe::getAndBitwiseOrIntRelease(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            return getAndBitwiseAndInt(o, offset, mask);
        }

        gint Unsafe::getAndBitwiseOrIntRelaxed(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            return getAndBitwiseAndInt(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseOrLongAcquire(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            return getAndBitwiseAndLong(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseOrLongRelease(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            return getAndBitwiseAndLong(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseOrLongRelaxed(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_or)
            return __atomic_fetch_or((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            return getAndBitwiseAndLong(o, offset, mask);
        }

        gbool Unsafe::getAndBitwiseOrBoolean(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrByte(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseOrBooleanAcquire(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrByteAcquire(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseOrBooleanRelease(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrByteRelease(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseOrBooleanRelaxed(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrByteRelaxed(o, offset, mask ? 1 : 0);
        }

        gchar Unsafe::getAndBitwiseOrChar(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrShort(o, offset, mask);
        }

        gchar Unsafe::getAndBitwiseOrCharAcquire(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrShortAcquire(o, offset, mask);
        }

        gchar Unsafe::getAndBitwiseOrCharRelease(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrShortRelease(o, offset, mask);
        }

        gchar Unsafe::getAndBitwiseOrCharRelaxed(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseOrShortRelaxed(o, offset, mask);
        }

        gbyte Unsafe::getAndBitwiseAndByte(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            gbyte current;
            do {
                current = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current & mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseAndByteAcquire(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            gbyte current;
            do {
                current = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current & mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseAndByteRelease(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            gbyte current;
            do {
                current = getByte(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current & mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseAndByteRelaxed(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            gbyte current;
            do {
                current = getByteRelaxed(o, offset);
            } while (!weakCompareAndSetByteRelaxed(o, offset, current, (gbyte) (current & mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseAndShort(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            gshort current;
            do {
                current = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current & mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseAndShortAcquire(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            gshort current;
            do {
                current = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current & mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseAndShortRelease(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            gshort current;
            do {
                current = getShort(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current & mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseAndShortRelaxed(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            gshort current;
            do {
                current = getShortRelaxed(o, offset);
            } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gshort) (current & mask)));
            return current;
        }

        gint Unsafe::getAndBitwiseAndIntAcquire(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            return getAndBitwiseAndInt(o, offset, mask);
        }

        gint Unsafe::getAndBitwiseAndIntRelease(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            return getAndBitwiseAndInt(o, offset, mask);
        }

        gint Unsafe::getAndBitwiseAndIntRelaxed(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            return getAndBitwiseAndInt(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseAndLongAcquire(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            return getAndBitwiseAndLong(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseAndLongRelease(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            return getAndBitwiseAndLong(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseAndLongRelaxed(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_and)
            return __atomic_fetch_and((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            return getAndBitwiseAndLong(o, offset, mask);
        }

        gbool Unsafe::getAndBitwiseAndBoolean(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndByte(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseAndBooleanAcquire(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndByteAcquire(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseAndBooleanRelease(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndByteRelease(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseAndBooleanRelaxed(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndByteRelaxed(o, offset, mask ? 1 : 0);
        }

        gchar Unsafe::getAndBitwiseAndChar(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndShort(o, offset, mask);
        }

        gchar Unsafe::getAndBitwiseAndCharAcquire(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndShortAcquire(o, offset, mask);
        }

        gchar Unsafe::getAndBitwiseAndCharRelease(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndShortRelease(o, offset, mask);
        }

        gchar Unsafe::getAndBitwiseAndCharRelaxed(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseAndShortRelaxed(o, offset, mask);
        }

        gbyte Unsafe::getAndBitwiseXorByte(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            gbyte current;
            do {
                current = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current ^ mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseXorByteAcquire(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            gbyte current;
            do {
                current = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current ^ mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseXorByteRelease(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            gbyte current;
            do {
                current = getByte(o, offset);
            } while (!weakCompareAndSetByte(o, offset, current, (gbyte) (current ^ mask)));
            return current;
        }

        gbyte Unsafe::getAndBitwiseXorByteRelaxed(Object &o, glong offset, gbyte mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gbyte volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            gbyte current;
            do {
                current = getByteRelaxed(o, offset);
            } while (!weakCompareAndSetByteRelaxed(o, offset, current, (gbyte) (current ^ mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseXorShort(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 4);
#endif
            gshort current;
            do {
                current = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current ^ mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseXorShortAcquire(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            gshort current;
            do {
                current = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current ^ mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseXorShortRelease(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            gshort current;
            do {
                current = getShort(o, offset);
            } while (!weakCompareAndSetShort(o, offset, current, (gshort) (current ^ mask)));
            return current;
        }

        gshort Unsafe::getAndBitwiseXorShortRelaxed(Object &o, glong offset, gshort mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gshort volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            gshort current;
            do {
                current = getShortRelaxed(o, offset);
            } while (!weakCompareAndSetShortRelaxed(o, offset, current, (gshort) (current ^ mask)));
            return current;
        }

        gint Unsafe::getAndBitwiseXorIntAcquire(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            return getAndBitwiseXorInt(o, offset, mask);
        }

        gint Unsafe::getAndBitwiseXorIntRelease(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            return getAndBitwiseXorInt(o, offset, mask);
        }

        gint Unsafe::getAndBitwiseXorIntRelaxed(Object &o, glong offset, gint mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((gint volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            return getAndBitwiseXorInt(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseXorLongAcquire(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 2);
#endif
            return getAndBitwiseXorLong(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseXorLongRelease(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 3);
#endif
            return getAndBitwiseXorLong(o, offset, mask);
        }

        glong Unsafe::getAndBitwiseXorLongRelaxed(Object &o, glong offset, glong mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
#if __has_builtin(__atomic_fetch_xor)
            return __atomic_fetch_xor((glong volatile*) UnsafeImpl::getNativeAddress(o, offset), mask, 0);
#endif
            return getAndBitwiseXorLong(o, offset, mask);
        }

        gbool Unsafe::getAndBitwiseXorBoolean(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorByte(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseXorBooleanAcquire(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorByteAcquire(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseXorBooleanRelease(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorByteRelease(o, offset, mask ? 1 : 0) != 0;
        }

        gbool Unsafe::getAndBitwiseXorBooleanRelaxed(Object &o, glong offset, gbool mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorByteRelaxed(o, offset, mask ? 1 : 0);
        }

        gchar Unsafe::getAndBitwiseXorChar(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorShort(o, offset, (gshort) mask);
        }

        gchar Unsafe::getAndBitwiseXorCharAcquire(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorShortAcquire(o, offset, (gshort) mask);
        }

        gchar Unsafe::getAndBitwiseXorCharRelease(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorShortRelease(o, offset, (gshort) mask);
        }

        gchar Unsafe::getAndBitwiseXorCharRelaxed(Object &o, glong offset, gchar mask) {
            if (UnsafeImpl::checkPointer(o, offset))
                ArgumentException("Invalid input").throws(__trace("core.native.Unsafe"));
            return getAndBitwiseXorShortRelaxed(o, offset, (gshort) mask);
        }

    }

} // core