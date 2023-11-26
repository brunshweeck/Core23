//
// Created by T.N.Brunshweeck on 24/11/2023.
//

#ifndef CORE23_ARRAYSSUPPORT_H
#define CORE23_ARRAYSSUPPORT_H

#include <core/Object.h>
#include <core/ArgumentException.h>
#include <core/Integer.h>
#include "Preconditions.h"

namespace core {
    namespace util {

        /**
         * Utility methods to work with arrays.  This includes a set of methods
         * to find a mismatch between two primitive arrays.  Also included is
         * a method to calculate the new length of an array to be reallocated.
         *
         * <p>
         * Array equality and lexicographical comparison can be built on top of
         * array mismatch functionality.
         *
         * <p>
         * The mismatch method implementation, vectorizedMismatch, leverages
         * vector-based techniques to access and compare the contents of two arrays.
         * The Java implementation uses Unsafe.getLongUnaligned</b> to access the
         * content of an array, thus access is supported on platforms that do not
         * support unaligned access.  For a const ByteArray & array, 8 bytes (64 bits) can be
         * accessed and compared as a unit rather than individually, which increases
         * the performance when the method is compiled by the HotSpot VM.  On supported
         * platforms the mismatch implementation is intrinsified to leverage SIMD
         * instructions.  So for a const ByteArray & array, 16 bytes (128 bits), 32 bytes
         * (256 bits), and perhaps in the future even 64 bytes (512 bits), platform
         * permitting, can be accessed and compared as a unit, which further increases
         * the performance over the Java implementation.
         *
         * <p>None of the mismatch methods perform array bounds checks.  It is the
         * responsibility of the caller (direct or otherwise) to perform such checks
         * before calling this method.
         */
        class ArraysSupport final: public virtual Object {
        private:
            ArraysSupport() = default;

            CORE_DISABLE_COPY_MOVE(ArraysSupport);

            class Unsafe;

        public:
            /**
             * Find the relative index of the first mismatching pair of elements in two
             * primitive arrays of the same component type.  Pairs of elements will be
             * tested in order relative to given offsets into both arrays.
             *
             * <p>This method does not perform type checks or bounds checks.  It is the
             * responsibility of the caller to perform such checks before calling this
             * method.
             *
             * <p>The given offsets, in bytes, need not be aligned according to the
             * given log<sub>2</sub> size the array elements.  More specifically, an
             * offset modulus the size need not be zero.
             *
             * @param a the first array to be tested for mismatch, or null for
             * direct memory access
             * @param aOffset the relative offset, in bytes, from the base address of
             * the first array to test from, otherwise if the first array is
             * null, an absolute address pointing to the first element to test.
             * @param b the second array to be tested for mismatch, or null for
             * direct memory access
             * @param bOffset the relative offset, in bytes, from the base address of
             * the second array to test from, otherwise if the second array is
             * null, an absolute address pointing to the first element to test.
             * @param length the number of array elements to test
             * @param log2ArrayIndexScale log<sub>2</sub> of the array index scale, that
             * corresponds to the size, in bytes, of an array element.
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

            static gint mismatch(const BooleanArray &a, gint aFromIndex, const BooleanArray &b, gint bFromIndex, gint length);


            // Bytes

            /**
             * Find the index of a mismatch between two arrays.
             *
             * <p>This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @param length the number of bytes from each array to check
             * @return the index of a mismatch between the two arrays, otherwise -1 if
             * no mismatch.  The index will be within the range of (inclusive) 0 to
             * (exclusive) the smaller of the two array lengths.
             */
            static gint mismatch(const ByteArray &a, const ByteArray &b, gint length);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(const ByteArray &a, gint aFromIndex, const ByteArray &b, gint bFromIndex, gint length);


            // Chars

            static gint mismatch(const CharArray &a, const CharArray &b, gint length);

            static gint mismatch(const CharArray &a, gint aFromIndex, const CharArray &b, gint bFromIndex, gint length);


            // Shorts

            static gint mismatch(const ShortArray &a, const ShortArray &b, gint length);

            static gint mismatch(const ShortArray &a, gint aFromIndex, const ShortArray &b, gint bFromIndex, gint length);


            // Ints
            static gint mismatch(const IntArray &a, const IntArray &b, gint length);

            static gint mismatch(const IntArray &a, gint aFromIndex, const IntArray &b, gint bFromIndex, gint length);


            // Floats
            static gint mismatch(const FloatArray &a, const FloatArray &b, gint length);

            static gint mismatch(const FloatArray &a, gint aFromIndex, const FloatArray &b, gint bFromIndex, gint length);

            // 64 bit sizes

            // Long
            static gint mismatch(const LongArray &a, const LongArray &b, gint length);

            static gint mismatch(const LongArray &a, gint aFromIndex, const LongArray &b, gint bFromIndex, gint length);


            // Double
            static gint mismatch(const DoubleArray &a, const DoubleArray &b, gint length);

            static gint mismatch(const DoubleArray &a, gint aFromIndex, const DoubleArray &b, gint bFromIndex, gint length);

            // copy


            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source
             * array referenced by <b> src</b> to the destination array
             * referenced by <b> dest</b>. The number of components copied is
             * equal to the <b> length</b> argument. The components at
             * positions <b> srcPos</b> through
             * <b> srcPos+length-1</b> in the source array are copied into
             * positions <b> destPos</b> through
             * <b> destPos+length-1</b>, respectively, of the destination
             * array.
             * <p>
             * If the <b> src</b> and <b> dest</b> arguments refer to the
             * same array object, then the copying is performed as if the
             * components at positions <b> srcPos</b> through
             * <b> srcPos+length-1</b> were first copied to a temporary
             * array with <b> length</b> components and then the contents of
             * the temporary array were copied into positions
             * <b> destPos</b> through <b> destPos+length-1</b> of the
             * destination array.
             * <p>
             * If <b> dest</b> is <b> null</b>, then a
             * <b> ArgumentException</b> is thrown.
             * <p>
             * If <b> src</b> is <b> null</b>, then a
             * <b> ArgumentException</b> is thrown and the destination
             * array is not modified.
             * <p>
             * Otherwise, if any of the following is true, an
             * <b> AssertionException</b> is thrown and the destination is
             * not modified:
             * <ul>
             * <li>The <b> src</b> argument refers to an object that is not an
             *     array.
             * <li>The <b> dest</b> argument refers to an object that is not an
             *     array.
             * <li>The <b> src</b> argument and <b> dest</b> argument refer
             *     to arrays whose component types are different primitive types.
             * <li>The <b> src</b> argument refers to an array with a primitive
             *    component type and the <b> dest</b> argument refers to an array
             *     with a reference component type.
             * <li>The <b> src</b> argument refers to an array with a reference
             *    component type and the <b> dest</b> argument refers to an array
             *     with a primitive component type.
             * </ul>
             * <p>
             * Otherwise, if any of the following is true, an
             * <b> IndexOutOfBoundsException</b> is
             * thrown and the destination is not modified:
             * <ul>
             * <li>The <b> srcPos</b> argument is negative.
             * <li>The <b> destPos</b> argument is negative.
             * <li>The <b> length</b> argument is negative.
             * <li><b> srcPos+length</b> is greater than
             *     <b> src.length</b>, the length of the source array.
             * <li><b> destPos+length</b> is greater than
             *     <b> dest.length</b>, the length of the destination array.
             * </ul>
             * <p>
             * Otherwise, if any actual component of the source array from
             * position <b> srcPos</b> through
             * <b> srcPos+length-1</b> cannot be converted to the component
             * type of the destination array by assignment conversion, an
             * <b> ArrayStoreException</b> is thrown. In this case, let
             * <b><i>k</i></b> be the smallest nonnegative integer less than
             * length such that <b> src[srcPos+</b><i>k</i><b> ]</b>
             * cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array components from
             * positions <b> srcPos</b> through
             * <b> srcPos+</b><i>k</i><b> -1</b>
             * will already have been copied to destination array positions
             * <b> destPos</b> through
             * <b> destPos+</b><i>k</I><b> -1</b> and no other
             * positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this
             * paragraph effectively applies only to the situation where both
             * arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcPos   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destPos  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexException  if copying would cause
             *             access of data outside array bounds.
             * @throws     AssertionError  if an element in the <b> src</b>
             *             array could not be stored into the <b> dest</b> array
             *             because of a type mismatch.
             * @throws     ArgumentException if either <b> src</b> or
             *             <b> dest</b> is <b> null</b>.
             */
            template<class T, class K>
            static void copy(T &&src, gint srcPos, K &&dest, gint destPos, gint length) {
                CORE_STATIC_ASSERT((Class<T>::isArray() || Class<T>::isPointer()) && (Class<K>::isArray() || Class<K>::isPointer()),"Illegal arguments");
                CORE_STATIC_ASSERT(
                        Class<typename Class<K>::NoPointer>::template isAssignable<typename Class<T>::NoPointer>() ||
                        Class<typename Class<K>::NoPointer>::template isAssignable<typename Class<T>::NoArray>() ||
                        Class<typename Class<K>::NoArray>::template isAssignable<typename Class<T>::NoPointer>() ||
                        Class<typename Class<K>::NoArray>::template isAssignable<typename Class<T>::NoArray>(),
                        "Couldn't store src array to dest array: type mismatch");

                if (length < 0)
                    ArgumentException("Negative length").throws(__trace("Core::ArraysSupport"));
                try {
                    if (Class<T>::isArray()) {
                        gint size = sizeof(T) / sizeof(src[0]);
                        Preconditions::checkIndexFromRange(srcPos, srcPos + length, size);
                    }
                    if (Class<K>::isArray()) {
                        gint size = sizeof(T) / sizeof(dest[0]);
                        Preconditions::checkIndexFromRange(destPos, destPos + length, size);
                    }
                } catch (const Throwable &thr) {
                    thr.throws(__trace("Core::ArraysSupport"));
                }
                if (src && dest) {
                    if (srcPos < destPos) {
                        for (int i = length; i > 0; i--)
                            dest[destPos + i - 1] = src[srcPos + i - 1];
                    } else
                        for (int i = 0; i < length; ++i) {
                            dest[destPos + i] = src[srcPos + i];
                        }
                } else
                    ArgumentException("Null pointer").throws(__trace("Core::ArraysSupport"));
            }

            /**
             * A soft maximum array length imposed by array growth computations.
             */
            static CORE_FAST gint SOFT_MAX_ARRAY_LENGTH = Integer::MAX_VALUE - 8;

            /**
             * Computes a new array length given an array's current length, a minimum growth
             * amount, and a preferred growth amount. The computation is done in an overflow-safe
             * fashion.
             *
             * This method is used by objects that contain an array that might need to be grown
             * in order to fulfill some immediate need (the minimum growth amount) but would also
             * like to request more space (the preferred growth amount) in order to accommodate
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
             * Note that this method does not do any array allocation itself; it only does array
             * length growth computations. However, it will throw MemoryError as noted above.
             *
             * Note also that this method cannot detect the implementation limit, and it
             * may compute and return a length value up to and including Integer.MAX_VALUE that
             * might exceed the implementation limit. In that case, the caller will likely
             * attempt an array allocation with that length and encounter an MemoryError.
             * Of course, regardless of the length value returned from this method, the caller
             * may encounter MemoryError if there is insufficient heap to fulfill the request.
             *
             * @param oldLength   current length of the array (must be non-negative)
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
