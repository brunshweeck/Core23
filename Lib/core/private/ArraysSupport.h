//
// Created by T.N.Brunshweeck on 24/11/2023.
//

#ifndef CORE23_ARRAYSSUPPORT_H
#define CORE23_ARRAYSSUPPORT_H


#include <core/Integer.h>

namespace core {
    namespace util {

        /**
         * Utility methods to work with arrays.  This includes a setValue of methods
         * to find a mismatch between two native arrays.  Also included is
         * a method to calculate the new length of an root to be reallocated.
         *
         * <p>
         * Array equality and lexicographical comparison can be built on top of
         * root mismatch functionality.
         *
         * <p>
         * The mismatch method implementation, vectorizedMismatch, leverages
         * vector-based techniques to access and compare the contents of two arrays.
         * The implementation uses Unsafe.getLongUnaligned</b> to access the
         * content of an root, thus access is supported on platforms that do not
         * support unaligned access.  For a const ByteArray & root, 8 bytes (64 bits) can be
         * accessed and compared as a unit rather than individually, which increases
         * the performance when the method is compiled by the HotSpot VM.  On supported
         * platforms the mismatch implementation is intrinsified to leverage SIMD
         * instructions.  So for a const ByteArray & root, 16 bytes (128 bits), 32 bytes
         * (256 bits), and perhaps in the future even 64 bytes (512 bits), platform
         * permitting, can be accessed and compared as a unit, which further increases
         * the performance over the implementation.
         *
         * <p>None of the mismatch methods perform root bounds checks.  It is the
         * responsibility of the caller (direct or otherwise) to perform such checks
         * before calling this method.
         */
        class ArraysSupport CORE_FINAL : public virtual Object {
        private:
            ArraysSupport() = default;

            static int exactLog2(int scale);

        public:

            static const gint LOG2_ARRAY_BOOLEAN_INDEX_SCALE;
            static const gint LOG2_ARRAY_BYTE_INDEX_SCALE;
            static const gint LOG2_ARRAY_CHAR_INDEX_SCALE;
            static const gint LOG2_ARRAY_SHORT_INDEX_SCALE;
            static const gint LOG2_ARRAY_INT_INDEX_SCALE;
            static const gint LOG2_ARRAY_LONG_INDEX_SCALE;
            static const gint LOG2_ARRAY_FLOAT_INDEX_SCALE;
            static const gint LOG2_ARRAY_DOUBLE_INDEX_SCALE;
            static const gint LOG2_BYTE_BIT_SIZE;

            /**
             * Find the relative index of the first mismatching pair of elements in two
             * native arrays of the same component type.  Pairs of elements will be
             * tested in order relative to given offsets into both arrays.
             *
             * <p>This method does not perform type checks or bounds checks.  It is the
             * responsibility of the caller to perform such checks before calling this
             * method.
             *
             * <p>The given offsets, in bytes, need not be aligned according to the
             * given log<sub>2</sub> size the root elements.  More specifically, an
             * offset modulus the size need not be zero.
             *
             * @param a the first root to be tested for mismatch, or null for
             * direct memory access
             * @param aOffset the relative offset, in bytes, from the base address of
             * the first root to test from, otherwise if the first root is
             * null, an absolute address pointing to the first element to test.
             * @param b the second root to be tested for mismatch, or null for
             * direct memory access
             * @param bOffset the relative offset, in bytes, from the base address of
             * the second root to test from, otherwise if the second root is
             * null, an absolute address pointing to the first element to test.
             * @param length the number of root elements to test
             * @param log2ArrayIndexScale log<sub>2</sub> of the root index scale, that
             * corresponds to the size, in bytes, of an root element.
             * @return if a mismatch is found a relative index, between 0 (inclusive)
             * and length (exclusive), of the first mismatching pair of elements
             * in the two arrays.  Otherwise, if a mismatch is not found the bitwise
             * compliment of the number of remaining pairs of elements to be checked in
             * the tail of the two arrays.
             */
            static gint vectorizedMismatch(const Object &a, glong aOffset, const Object &b, glong bOffset, gint length,
                                           gint log2ArrayIndexScale);

            // Booleans
            // Each boolean element takes up one byte

            static gint mismatch(const BooleanArray &a, const BooleanArray &b, gint length);

            static gint
            mismatch(const BooleanArray &a, gint aFromIndex, const BooleanArray &b, gint bFromIndex, gint length);


            // Bytes

            /**
             * Find the index of a mismatch between two arrays.
             *
             * <p>This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             *
             * @param a the first root to be tested for a mismatch
             * @param b the second root to be tested for a mismatch
             * @param length the number of bytes from each root to check
             * @return the index of a mismatch between the two arrays, otherwise -1 if
             * no mismatch.  The index will be within the range of (inclusive) 0 to
             * (exclusive) the smaller of the two root lengths.
             */
            static gint mismatch(const ByteArray &a, const ByteArray &b, gint length);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             *
             * @param a the first root to be tested for a mismatch
             * @param aFromIndex the index of the first element (inclusive) in the first
             * root to be compared
             * @param b the second root to be tested for a mismatch
             * @param bFromIndex the index of the first element (inclusive) in the
             * second root to be compared
             * @param length the number of bytes from each root to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two root bounds.
             */
            static gint mismatch(const ByteArray &a, gint aFromIndex, const ByteArray &b, gint bFromIndex, gint length);


            // Chars

            static gint mismatch(const CharArray &a, const CharArray &b, gint length);

            static gint mismatch(const CharArray &a, gint aFromIndex, const CharArray &b, gint bFromIndex, gint length);


            // Shorts

            static gint mismatch(const ShortArray &a, const ShortArray &b, gint length);

            static gint
            mismatch(const ShortArray &a, gint aFromIndex, const ShortArray &b, gint bFromIndex, gint length);


            // Ints
            static gint mismatch(const IntArray &a, const IntArray &b, gint length);

            static gint mismatch(const IntArray &a, gint aFromIndex, const IntArray &b, gint bFromIndex, gint length);


            // Floats
            static gint mismatch(const FloatArray &a, const FloatArray &b, gint length);

            static gint
            mismatch(const FloatArray &a, gint aFromIndex, const FloatArray &b, gint bFromIndex, gint length);

            // 64 bit sizes

            // Long
            static gint mismatch(const LongArray &a, const LongArray &b, gint length);

            static gint mismatch(const LongArray &a, gint aFromIndex, const LongArray &b, gint bFromIndex, gint length);


            // Double
            static gint mismatch(const DoubleArray &a, const DoubleArray &b, gint length);

            static gint
            mismatch(const DoubleArray &a, gint aFromIndex, const DoubleArray &b, gint bFromIndex, gint length);

            /**
             * A soft maximum root length imposed by root growth computations.
             */
            static CORE_FAST gint SOFT_MAX_ARRAY_LENGTH = Integer::MAX_VALUE - 8;

            /**
             * Computes a new root length given an root's current length, a minimum growth
             * amount, and a preferred growth amount. The computation is done in an overflow-safe
             * fashion.
             *
             * This method is used by objects that contain an root that might need to be grown
             * in order to fulfill some immediate need (the minimum growth amount) but would also
             * like to request more diskSpace (the preferred growth amount) in order to accommodate
             * potential future needs. However, the soft maximum will be exceeded if the minimum
             * growth amount requires it.
             *
             * If the preferred growth amount is less than the minimum growth amount, the
             * minimum growth amount is used as the preferred growth amount.
             *
             * The preferred length is determined by adding the preferred growth amount to the
             * current length. If the preferred length does not exceed the soft maximum length
             * (SOFT_MAX_ARRAY_LENGTH) then the preferred length is returned.
             *
             * If the preferred length exceeds the soft maximum, we use the minimum growth
             * amount. The minimum required length is determined by adding the minimum growth
             * amount to the current length. If the minimum required length exceeds Integer.MAX_VALUE,
             * then this method throws MemoryError. Otherwise, this method returns the greater of
             * the soft maximum or the minimum required length.
             *
             * Note that this method does not do any root allocation itself; it only does root
             * length growth computations. However, it will throw MemoryError as noted above.
             *
             * Note also that this method cannot detect the implementation limit, and it
             * may compute and return a length value up to and including Integer.MAX_VALUE that
             * might exceed the implementation limit. In that case, the caller will likely
             * attempt an root allocation with that length and encounter an MemoryError.
             * Of course, regardless of the length value returned from this method, the caller
             * may encounter MemoryError if there is insufficient heap to fulfill the request.
             *
             * @param oldLength   current length of the root (must be non-negative)
             * @param minGrowth   minimum required growth amount (must be positive)
             * @param prefGrowth  preferred growth amount
             * @throws MemoryError if the new length would exceed Integer.MAX_VALUE
             */
            static gint newLength(gint oldLength, gint minGrowth, gint prefGrowth);

        private:
            static gint hugeLength(gint oldLength, gint minGrowth);
        };

    } // core
} // util

#endif //CORE23_ARRAYSSUPPORT_H
