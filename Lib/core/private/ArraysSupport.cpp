//
// Created by T.N.Brunshweeck on 24/11/2023.
//

#include "ArraysSupport.h"
#include <core/native/BooleanArray.h>
#include <core/native/ByteArray.h>
#include <core/native/ShortArray.h>
#include <core/native/IntArray.h>
#include <core/native/LongArray.h>
#include <core/native/FloatArray.h>
#include <core/native/DoubleArray.h>
#include <core/Boolean.h>
#include <core/Byte.h>
#include <core/Short.h>
#include <core/Character.h>
#include <core/Integer.h>
#include <core/Long.h>
#include <core/Float.h>
#include <core/Double.h>
#include <core/MemoryError.h>
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    namespace util {

        static CORE_FAST gint LOG2_ARRAY_BOOLEAN_INDEX_SCALE = 0;
        static CORE_FAST gint LOG2_ARRAY_BYTE_INDEX_SCALE = 0;
        static CORE_FAST gint LOG2_ARRAY_CHAR_INDEX_SCALE = 1;
        static CORE_FAST gint LOG2_ARRAY_SHORT_INDEX_SCALE = 1;
        static CORE_FAST gint LOG2_ARRAY_INT_INDEX_SCALE = 2;
        static CORE_FAST gint LOG2_ARRAY_LONG_INDEX_SCALE = 3;
        static CORE_FAST gint LOG2_ARRAY_FLOAT_INDEX_SCALE = 2;
        static CORE_FAST gint LOG2_ARRAY_DOUBLE_INDEX_SCALE = 3;
        static CORE_FAST gint ARRAY_BOOLEAN_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_BYTE_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_SHORT_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_INT_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_LONG_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_FLOAT_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_DOUBLE_BASE_OFFSET = 24;
        static CORE_FAST gint ARRAY_CHAR_BASE_OFFSET = 24;

        static CORE_FAST gint LOG2_BYTE_BIT_SIZE = 0;

        interface ArraysSupport::Unsafe {

            static glong getLongUnaligned(const Object &o, glong offset) {
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

            static glong getAddress(const Object &object) {
                glong address = 0;
                if (Class<BooleanArray>::hasInstance(object)) {
                    address = (glong) ((BooleanArray &) object).value;
                } else if (Class<ByteArray>::hasInstance(object)) {
                    address = (glong) ((ByteArray &) object).value;
                } else if (Class<ShortArray>::hasInstance(object)) {
                    address = (glong) ((ShortArray &) object).value;
                } else if (Class<CharArray>::hasInstance(object)) {
                    address = (glong) ((CharArray &) object).value;
                } else if (Class<IntArray>::hasInstance(object)) {
                    address = (glong) ((IntArray &) object).value;
                } else if (Class<LongArray>::hasInstance(object)) {
                    address = (glong) ((LongArray &) object).value;
                } else if (Class<FloatArray>::hasInstance(object)) {
                    address = (glong) ((FloatArray &) object).value;
                } else if (Class<DoubleArray>::hasInstance(object)) {
                    address = (glong) ((DoubleArray &) object).value;
                }
                return address;
            }

            static gbyte getByte(const Object &object, glong offset) {
                return *(gbyte *) (getAddress(object) + offset);
            }

            static gshort getShort(const Object &object, glong offset) {
                return *(gshort *) (getAddress(object) + offset);
            }

            static gint getInt(const Object &object, glong offset) {
                return *(gint *) (getAddress(object) + offset);
            }

            static glong getLong(const Object &object, glong offset) {
                return *(glong *) (getAddress(object) + offset);
            }

            static gchar convEndian(gbool big, gchar n) { return big == U::BIG_ENDIAN ? n : Character::reverseBytes(n); }

            static gshort convEndian(gbool big, gshort n) { return big == U::BIG_ENDIAN ? n : Short::reverseBytes(n); }

            static gint convEndian(gbool big, gint n) { return big == U::BIG_ENDIAN ? n : Integer::reverseBytes(n); }

            static glong convEndian(gbool big, glong n) { return big == U::BIG_ENDIAN ? n : Long::reverseBytes(n); }

            static glong getLongUnaligned(const Object &o, glong offset, bool bigEndian) {
                return convEndian(bigEndian, getLongUnaligned(o, offset));
            }

            static gint getIntUnaligned(const Object &o, glong offset) {
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

            static gint getIntUnaligned(const Object &o, glong offset, gbool bigEndian) {
                return convEndian(bigEndian, getIntUnaligned(o, offset));
            }

            static glong makeLong(gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5, gbyte i6, gbyte i7) {
                return ((toUnsignedLong(i0) << pickPos(56, 0))
                        | (toUnsignedLong(i1) << pickPos(56, 8))
                        | (toUnsignedLong(i2) << pickPos(56, 16))
                        | (toUnsignedLong(i3) << pickPos(56, 24))
                        | (toUnsignedLong(i4) << pickPos(56, 32))
                        | (toUnsignedLong(i5) << pickPos(56, 40))
                        | (toUnsignedLong(i6) << pickPos(56, 48))
                        | (toUnsignedLong(i7) << pickPos(56, 56)));
            }

            static glong makeLong(gshort i0, gshort i1, gshort i2, gshort i3) {
                return ((toUnsignedLong(i0) << pickPos(48, 0))
                        | (toUnsignedLong(i1) << pickPos(48, 16))
                        | (toUnsignedLong(i2) << pickPos(48, 32))
                        | (toUnsignedLong(i3) << pickPos(48, 48)));
            }

            static glong makeLong(gint i0, gint i1) {
                return (toUnsignedLong(i0) << pickPos(32, 0))
                       | (toUnsignedLong(i1) << pickPos(32, 32));
            }

            static gint makeInt(gshort i0, gshort i1) {
                return (toUnsignedInt(i0) << pickPos(16, 0))
                       | (toUnsignedInt(i1) << pickPos(16, 16));
            }

            static gint makeInt(gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
                return ((toUnsignedInt(i0) << pickPos(24, 0))
                        | (toUnsignedInt(i1) << pickPos(24, 8))
                        | (toUnsignedInt(i2) << pickPos(24, 16))
                        | (toUnsignedInt(i3) << pickPos(24, 24)));
            }

            static gint toUnsignedInt(gbyte n) { return n & 0xff; }

            static gint toUnsignedInt(gshort n) { return n & 0xffff; }

            static glong toUnsignedLong(gbyte n) { return n & 0xffL; }

            static glong toUnsignedLong(gshort n) { return n & 0xffffL; }

            static glong toUnsignedLong(gint n) { return n & 0xffffffffL; }

            static gint pickPos(gint top, gint pos) { return U::BIG_ENDIAN ? top - pos : pos; }
        };

        gint ArraysSupport::vectorizedMismatch(const Object &a, glong aOffset, const Object &b, glong bOffset, gint length,
                                               gint log2ArrayIndexScale) {
            // assert a.getClass().isArray();
            // assert b.getClass().isArray();
            // assert 0 <= length <= sizeOf(a)
            // assert 0 <= length <= sizeOf(b)
            // assert 0 <= log2ArrayIndexScale <= 3

            gint log2ValuesPerWidth = LOG2_ARRAY_LONG_INDEX_SCALE - log2ArrayIndexScale;
            gint wi = 0;
            for (; wi < length >> log2ValuesPerWidth; wi++) {
                glong bi = ((glong) wi) << LOG2_ARRAY_LONG_INDEX_SCALE;
                glong av = U::getLongUnaligned(a, aOffset + bi);
                glong bv = U::getLongUnaligned(b, bOffset + bi);
                if (av != bv) {
                    glong x = av ^ bv;
                    gint o = U::BIG_ENDIAN
                             ? Long::leadingZeros(x) >> (LOG2_BYTE_BIT_SIZE + log2ArrayIndexScale)
                             : Long::trailingZeros(x) >> (LOG2_BYTE_BIT_SIZE + log2ArrayIndexScale);
                    return (wi << log2ValuesPerWidth) + o;
                }
            }

            // Calculate the tail of remaining elements to check
            gint tail = length - (wi << log2ValuesPerWidth);

            if (log2ArrayIndexScale < LOG2_ARRAY_INT_INDEX_SCALE) {
                gint wordTail = 1 << (LOG2_ARRAY_INT_INDEX_SCALE - log2ArrayIndexScale);
                // Handle 4 bytes or 2 chars in the tail using gint width
                if (tail >= wordTail) {
                    glong bi = ((glong) wi) << LOG2_ARRAY_LONG_INDEX_SCALE;
                    gint av = U::getIntUnaligned(a, aOffset + bi);
                    gint bv = U::getIntUnaligned(b, bOffset + bi);
                    if (av != bv) {
                        gint x = av ^ bv;
                        gint o = U::BIG_ENDIAN
                                 ? Integer::leadingZeros(x) >> (LOG2_BYTE_BIT_SIZE + log2ArrayIndexScale)
                                 : Integer::trailingZeros(x) >> (LOG2_BYTE_BIT_SIZE + log2ArrayIndexScale);
                        return (wi << log2ValuesPerWidth) + o;
                    }
                    tail -= wordTail;
                }
                return ~tail;
            } else {
                return ~tail;
            }
        }

        gint ArraysSupport::mismatch(const BooleanArray &a, const BooleanArray &b, gint length) {
            gint i = 0;
            if (length > 7) {
                if (a[0] != b[0])
                    return 0;
                Boolean bb;
                i = vectorizedMismatch(
                        a, ARRAY_BOOLEAN_BASE_OFFSET,
                        b, ARRAY_BOOLEAN_BASE_OFFSET,
                        length, LOG2_ARRAY_BOOLEAN_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[i] != b[i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const BooleanArray &a, gint aFromIndex, const BooleanArray &b, gint bFromIndex,
                                     gint length) {
            gint i = 0;
            if (length > 7) {
                if (a[aFromIndex] != b[bFromIndex])
                    return 0;
                gint aOffset = ARRAY_BOOLEAN_BASE_OFFSET + aFromIndex;
                gint bOffset = ARRAY_BOOLEAN_BASE_OFFSET + bFromIndex;
                i = vectorizedMismatch(
                        a, aOffset,
                        b, bOffset,
                        length, LOG2_ARRAY_BOOLEAN_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFromIndex + i] != b[bFromIndex + i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const ByteArray &a, const ByteArray &b, gint length) {
            // ISSUE: defer to index receiving methods if performance is good
            // assert length <= a.length
            // assert length <= b.length

            gint i = 0;
            if (length > 7) {
                if (a[0] != b[0])
                    return 0;
                i = vectorizedMismatch(
                        a, ARRAY_BYTE_BASE_OFFSET,
                        b, ARRAY_BYTE_BASE_OFFSET,
                        length, LOG2_ARRAY_BYTE_INDEX_SCALE);
                if (i >= 0)
                    return i;
                // Align to tail
                i = length - ~i;
//            assert i >= 0 && i <= 7;
            }
            // Tail < 8 bytes
            for (; i < length; i++) {
                if (a[i] != b[i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const ByteArray &a, gint aFromIndex, const ByteArray &b, gint bFromIndex,
                                     gint length) {
            // assert 0 <= aFromIndex < a.length
            // assert 0 <= aFromIndex + length <= a.length
            // assert 0 <= bFromIndex < b.length
            // assert 0 <= bFromIndex + length <= b.length
            // assert length >= 0

            gint i = 0;
            if (length > 7) {
                if (a[aFromIndex] != b[bFromIndex])
                    return 0;
                gint aOffset = ARRAY_BYTE_BASE_OFFSET + aFromIndex;
                gint bOffset = ARRAY_BYTE_BASE_OFFSET + bFromIndex;
                i = vectorizedMismatch(
                        a, aOffset,
                        b, bOffset,
                        length, LOG2_ARRAY_BYTE_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFromIndex + i] != b[bFromIndex + i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const CharArray &a, const CharArray &b, gint length) {
            gint i = 0;
            if (length > 3) {
                if (a[0] != b[0])
                    return 0;
                i = vectorizedMismatch(
                        a, ARRAY_CHAR_BASE_OFFSET,
                        b, ARRAY_CHAR_BASE_OFFSET,
                        length, LOG2_ARRAY_CHAR_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[i] != b[i])
                    return i;
            }
            return -1;
        }

        gint
        ArraysSupport::mismatch(const CharArray &a, gint aFromIndex, const CharArray &b, gint bFromIndex, gint length) {
            gint i = 0;
            if (length > 3) {
                if (a[aFromIndex] != b[bFromIndex])
                    return 0;
                gint aOffset = ARRAY_CHAR_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_CHAR_INDEX_SCALE);
                gint bOffset = ARRAY_CHAR_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_CHAR_INDEX_SCALE);
                i = vectorizedMismatch(
                        a, aOffset,
                        b, bOffset,
                        length, LOG2_ARRAY_CHAR_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFromIndex + i] != b[bFromIndex + i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const ShortArray &a, const ShortArray &b, gint length) {
            gint i = 0;
            if (length > 3) {
                if (a[0] != b[0])
                    return 0;
                i = vectorizedMismatch(
                        a, ARRAY_SHORT_BASE_OFFSET,
                        b, ARRAY_SHORT_BASE_OFFSET,
                        length, LOG2_ARRAY_SHORT_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[i] != b[i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const ShortArray &a, gint aFromIndex, const ShortArray &b, gint bFromIndex,
                                     gint length) {
            gint i = 0;
            if (length > 3) {
                if (a[aFromIndex] != b[bFromIndex])
                    return 0;
                gint aOffset = ARRAY_SHORT_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_SHORT_INDEX_SCALE);
                gint bOffset = ARRAY_SHORT_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_SHORT_INDEX_SCALE);
                i = vectorizedMismatch(
                        a, aOffset,
                        b, bOffset,
                        length, LOG2_ARRAY_SHORT_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFromIndex + i] != b[bFromIndex + i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const IntArray &a, const IntArray &b, gint length) {
            gint i = 0;
            if (length > 1) {
                if (a[0] != b[0])
                    return 0;
                i = vectorizedMismatch(
                        a, ARRAY_INT_BASE_OFFSET,
                        b, ARRAY_INT_BASE_OFFSET,
                        length, LOG2_ARRAY_INT_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[i] != b[i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const IntArray &a, gint aFromIndex, const IntArray &b, gint bFromIndex, gint length) {
            gint i = 0;
            if (length > 1) {
                if (a[aFromIndex] != b[bFromIndex])
                    return 0;
                gint aOffset = ARRAY_INT_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_INT_INDEX_SCALE);
                gint bOffset = ARRAY_INT_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_INT_INDEX_SCALE);
                i = vectorizedMismatch(
                        a, aOffset,
                        b, bOffset,
                        length, LOG2_ARRAY_INT_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFromIndex + i] != b[bFromIndex + i])
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const FloatArray &a, const FloatArray &b, gint length) {
            return mismatch(a, 0, b, 0, length);
        }

        gint ArraysSupport::mismatch(const FloatArray &a, gint aFromIndex, const FloatArray &b, gint bFromIndex,
                                     gint length) {
            gint i = 0;
            if (length > 1) {
                if (Float::toIntBits(a[aFromIndex]) == Float::toIntBits(b[bFromIndex])) {
                    gint aOffset = ARRAY_FLOAT_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_FLOAT_INDEX_SCALE);
                    gint bOffset = ARRAY_FLOAT_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_FLOAT_INDEX_SCALE);
                    i = vectorizedMismatch(
                            a, aOffset,
                            b, bOffset,
                            length, LOG2_ARRAY_FLOAT_INDEX_SCALE);
                }
                // Mismatched
                if (i >= 0) {
                    // Check if mismatch is not associated with two NaN values
                    if (!Float::isNaN(a[aFromIndex + i]) || !Float::isNaN(b[bFromIndex + i]))
                        return i;

                    // Mismatch on two different NaN values that are normalized to match
                    // Fall back to slow mechanism
                    // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                    // However, requires that returned value be relative to input ranges
                    i++;
                }
                    // Matched
                else {
                    i = length - ~i;
                }
            }
            for (; i < length; i++) {
                if (Float::toIntBits(a[aFromIndex + i]) != Float::toIntBits(b[bFromIndex + i]))
                    return i;
            }
            return -1;
        }

        gint ArraysSupport::mismatch(const LongArray &a, const LongArray &b, gint length) {
            if (length == 0) {
                return -1;
            }
            if (a[0] != b[0])
                return 0;
            gint i = vectorizedMismatch(
                    a, ARRAY_LONG_BASE_OFFSET,
                    b, ARRAY_LONG_BASE_OFFSET,
                    length, LOG2_ARRAY_LONG_INDEX_SCALE);
            return i >= 0 ? i : -1;
        }

        gint ArraysSupport::mismatch(const LongArray &a, gint aFromIndex, const LongArray &b, gint bFromIndex,
                                     gint length) {
            if (length == 0) {
                return -1;
            }
            if (a[aFromIndex] != b[bFromIndex])
                return 0;
            gint aOffset = ARRAY_LONG_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_LONG_INDEX_SCALE);
            gint bOffset = ARRAY_LONG_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_LONG_INDEX_SCALE);
            gint i = vectorizedMismatch(
                    a, aOffset,
                    b, bOffset,
                    length, LOG2_ARRAY_LONG_INDEX_SCALE);
            return i >= 0 ? i : -1;
        }

        gint ArraysSupport::mismatch(const DoubleArray &a, const DoubleArray &b, gint length) {
            return mismatch(a, 0, b, 0, length);
        }

        gint ArraysSupport::mismatch(const DoubleArray &a, gint aFromIndex, const DoubleArray &b, gint bFromIndex,
                                     gint length) {
            if (length == 0) {
                return -1;
            }
            gint i = 0;
            if (Double::toLongBits(a[aFromIndex]) == Double::toLongBits(b[bFromIndex])) {
                gint aOffset = ARRAY_DOUBLE_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_DOUBLE_INDEX_SCALE);
                gint bOffset = ARRAY_DOUBLE_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_DOUBLE_INDEX_SCALE);
                i = vectorizedMismatch(
                        a, aOffset,
                        b, bOffset,
                        length, LOG2_ARRAY_DOUBLE_INDEX_SCALE);
            }
            if (i >= 0) {
                // Check if mismatch is not associated with two NaN values
                if (!Double::isNaN(a[aFromIndex + i]) || !Double::isNaN(b[bFromIndex + i]))
                    return i;

                // Mismatch on two different NaN values that are normalized to match
                // Fall back to slow mechanism
                // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                // However, requires that returned value be relative to input ranges
                i++;
                for (; i < length; i++) {
                    if (Double::toLongBits(a[aFromIndex + i]) != Double::toLongBits(b[bFromIndex + i]))
                        return i;
                }
            }

            return -1;
        }

        gint ArraysSupport::newLength(gint oldLength, gint minGrowth, gint prefGrowth) {
            // preconditions not checked because of inlining
            // assert oldLength >= 0
            // assert minGrowth > 0

            gint prefLength = oldLength + Math::max(minGrowth, prefGrowth); // might overflow
            if (0 < prefLength && prefLength <= SOFT_MAX_ARRAY_LENGTH) {
                return prefLength;
            } else {
                // put code cold in a separate method
                try {
                    return hugeLength(oldLength, minGrowth);
                }catch(const Throwable& thr) {
                    thr.throws(__trace("Core::ArraysSupport"));
                }
            }
        }

        gint ArraysSupport::hugeLength(gint oldLength, gint minGrowth) {
            gint minLength = oldLength + minGrowth;
            if (minLength < 0) { // overflow
                MemoryError("Required array length " + String::valueOf(oldLength) + " + " + String::valueOf(minGrowth) +
                            " is too large").throws(__trace("Core::ArraysSupport"));
            } else if (minLength <= SOFT_MAX_ARRAY_LENGTH) {
                return SOFT_MAX_ARRAY_LENGTH;
            } else {
                return minLength;
            }
        }
    } // core
} // util