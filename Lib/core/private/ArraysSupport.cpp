//
// Created by T.N.Brunshweeck on 24/11/2023.
//

#include "ArraysSupport.h"
#include <core/native/BooleanArray.h>
#include <core/native/ByteArray.h>
#include <core/native/ShortArray.h>
#include <core/native/CharArray.h>
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

    namespace util {

        using namespace native;

        const gint ArraysSupport::LOG2_ARRAY_BOOLEAN_INDEX_SCALE = exactLog2(Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_BYTE_INDEX_SCALE = exactLog2(Unsafe::ARRAY_BYTE_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_CHAR_INDEX_SCALE = exactLog2(Unsafe::ARRAY_CHAR_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_SHORT_INDEX_SCALE = exactLog2(Unsafe::ARRAY_SHORT_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_INT_INDEX_SCALE = exactLog2(Unsafe::ARRAY_INT_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_LONG_INDEX_SCALE = exactLog2(Unsafe::ARRAY_LONG_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_FLOAT_INDEX_SCALE = exactLog2(Unsafe::ARRAY_FLOAT_INDEX_SCALE);
        const gint ArraysSupport::LOG2_ARRAY_DOUBLE_INDEX_SCALE = exactLog2(Unsafe::ARRAY_DOUBLE_INDEX_SCALE);
        const gint ArraysSupport::LOG2_BYTE_BIT_SIZE = exactLog2(sizeof(gbyte));

        gint
        ArraysSupport::vectorizedMismatch(const Object &a, glong aOffset, const Object &b, glong bOffset, gint length,
                                          gint log2ArrayIndexScale) {
            // assert a.getClass().isArray();
            // assert b.getClass().isArray();
            // assert 0 <= length <= sizeOf(a)
            // assert 0 <= length <= sizeOf(b)
            // assert 0 <= log2ArrayIndexScale <= 3

            gint const log2ValuesPerWidth = LOG2_ARRAY_LONG_INDEX_SCALE - log2ArrayIndexScale;
            gint wi = 0;
            for (; wi < length >> log2ValuesPerWidth; wi++) {
                glong const bi = ((glong) wi) << LOG2_ARRAY_LONG_INDEX_SCALE;
                glong const av = Unsafe::getLongUnaligned(a, aOffset + bi);
                glong const bv = Unsafe::getLongUnaligned(b, bOffset + bi);
                if (av != bv) {
                    glong const x = av ^ bv;
                    gint const o = Unsafe::BIG_ENDIAN
                                   ? Long::leadingZeros(x) >> (LOG2_BYTE_BIT_SIZE + log2ArrayIndexScale)
                                   : Long::trailingZeros(x) >> (LOG2_BYTE_BIT_SIZE + log2ArrayIndexScale);
                    return (wi << log2ValuesPerWidth) + o;
                }
            }

            // Calculate the tail of remaining elements to check
            gint tail = length - (wi << log2ValuesPerWidth);

            if (log2ArrayIndexScale < LOG2_ARRAY_INT_INDEX_SCALE) {
                gint const wordTail = 1 << (LOG2_ARRAY_INT_INDEX_SCALE - log2ArrayIndexScale);
                // Handle 4 bytes or 2 chars in the tail using gint width
                if (tail >= wordTail) {
                    glong const bi = ((glong) wi) << LOG2_ARRAY_LONG_INDEX_SCALE;
                    gint const av = Unsafe::getIntUnaligned(a, aOffset + bi);
                    gint const bv = Unsafe::getIntUnaligned(b, bOffset + bi);
                    if (av != bv) {
                        gint const x = av ^ bv;
                        gint const o = Unsafe::BIG_ENDIAN
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
                i = vectorizedMismatch(
                        a, Unsafe::ARRAY_BOOLEAN_BASE_OFFSET,
                        b, Unsafe::ARRAY_BOOLEAN_BASE_OFFSET,
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
                gint const aOffset = Unsafe::ARRAY_BOOLEAN_BASE_OFFSET + aFromIndex;
                gint const bOffset = Unsafe::ARRAY_BOOLEAN_BASE_OFFSET + bFromIndex;
                i = vectorizedMismatch(a, aOffset, b, bOffset,
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
                        a, Unsafe::ARRAY_BYTE_BASE_OFFSET,
                        b, Unsafe::ARRAY_BYTE_BASE_OFFSET,
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
                gint const aOffset = Unsafe::ARRAY_BYTE_BASE_OFFSET + aFromIndex;
                gint const bOffset = Unsafe::ARRAY_BYTE_BASE_OFFSET + bFromIndex;
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
                        a, Unsafe::ARRAY_CHAR_BASE_OFFSET,
                        b, Unsafe::ARRAY_CHAR_BASE_OFFSET,
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
                gint const aOffset = Unsafe::ARRAY_CHAR_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_CHAR_INDEX_SCALE);
                gint const bOffset = Unsafe::ARRAY_CHAR_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_CHAR_INDEX_SCALE);
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
                        a, Unsafe::ARRAY_SHORT_BASE_OFFSET,
                        b, Unsafe::ARRAY_SHORT_BASE_OFFSET,
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
                gint const aOffset = Unsafe::ARRAY_SHORT_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_SHORT_INDEX_SCALE);
                gint const bOffset = Unsafe::ARRAY_SHORT_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_SHORT_INDEX_SCALE);
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
                        a, Unsafe::ARRAY_INT_BASE_OFFSET,
                        b, Unsafe::ARRAY_INT_BASE_OFFSET,
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

        gint
        ArraysSupport::mismatch(const IntArray &a, gint aFromIndex, const IntArray &b, gint bFromIndex, gint length) {
            gint i = 0;
            if (length > 1) {
                if (a[aFromIndex] != b[bFromIndex])
                    return 0;
                gint const aOffset = Unsafe::ARRAY_INT_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_INT_INDEX_SCALE);
                gint const bOffset = Unsafe::ARRAY_INT_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_INT_INDEX_SCALE);
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
                    gint const aOffset = Unsafe::ARRAY_FLOAT_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_FLOAT_INDEX_SCALE);
                    gint const bOffset = Unsafe::ARRAY_FLOAT_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_FLOAT_INDEX_SCALE);
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
            gint const i = vectorizedMismatch(
                    a, Unsafe::ARRAY_LONG_BASE_OFFSET,
                    b, Unsafe::ARRAY_LONG_BASE_OFFSET,
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
            gint const aOffset = Unsafe::ARRAY_LONG_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_LONG_INDEX_SCALE);
            gint const bOffset = Unsafe::ARRAY_LONG_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_LONG_INDEX_SCALE);
            gint const i = vectorizedMismatch(
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
                gint const aOffset = Unsafe::ARRAY_DOUBLE_BASE_OFFSET + (aFromIndex << LOG2_ARRAY_DOUBLE_INDEX_SCALE);
                gint const bOffset = Unsafe::ARRAY_DOUBLE_BASE_OFFSET + (bFromIndex << LOG2_ARRAY_DOUBLE_INDEX_SCALE);
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
                // However, requires that the returned value be relative to input ranges
                i++;
                for (; i < length; i++) {
                    if (Double::toLongBits(a[aFromIndex + i]) != Double::toLongBits(b[bFromIndex + i]))
                        return i;
                }
            }

            return -1;
        }

        gint ArraysSupport::newLength(gint oldLength, gint minGrowth, gint prefGrowth) {
            // preconditions aren't checked because of inlining
            // assert oldLength >= 0
            // assert minGrowth > 0

            gint const prefLength = oldLength + Math::max(minGrowth, prefGrowth); // might overflow
            if (0 < prefLength && prefLength <= SOFT_MAX_ARRAY_LENGTH) {
                return prefLength;
            } else {
                // put code cold in a separate method
                try {
                    return hugeLength(oldLength, minGrowth);
                } catch (const Throwable &thr) {
                    thr.throws(__trace("Core.ArraysSupport"));
                }
            }
        }

        gint ArraysSupport::hugeLength(gint oldLength, gint minGrowth) {
            gint const minLength = oldLength + minGrowth;
            if (minLength < 0) { // overflow
                MemoryError("Required root length " + String::valueOf(oldLength) + " + " + String::valueOf(minGrowth) +
                            " is too large").throws(__trace("core.ArraysSupport"));
            } else if (minLength <= SOFT_MAX_ARRAY_LENGTH) {
                return SOFT_MAX_ARRAY_LENGTH;
            } else {
                return minLength;
            }
        }

        int ArraysSupport::exactLog2(int scale) {
            if ((scale & (scale - 1)) != 0)
                Error("data type scale not a power of two").throws(__trace("core.util.ArraySupport"));
            return Integer::trailingZeros(scale);
        }
    } // core
} // util