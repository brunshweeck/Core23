//
// Created by T.N.Brunshweeck on 01/02/2024.
//

#ifndef CORE23_ARRAYS_H
#define CORE23_ARRAYS_H

#include <core/Object.h>
#include <core/Comparable.h>
#include <core/native/ArrayStoreException.h>

namespace core {
    namespace util {

        /**
         * This class contains various methods for manipulating arrays (such as
         * sorting and searching). This class also contains a static factory
         * that allows arrays to be viewed as lists.
         *
         * <p>The documentation for the methods contained in this class includes
         * brief descriptions of the <i>implementations</i>. Such descriptions should
         * be regarded as <i>implementation notes</i>, rather than parts of the
         * <i>specification</i>. Implementors should feel free to substitute other
         * algorithms, so long as the specification itself is adhered to. (For
         * example, the algorithm used by <b> sort(ObjectArray)</b>  does not have to be
         * a MergeSort, but it does have to be <i>stable</i>.)
         *
         * <p>This class is a member of the <a href=""> Collections Framework</a>.
         *
         * @author Brunshweeck Tazeussong
         */
        class Arrays : public Object {
        private:
            Arrays() {}

        public:

            /*
             * Sorting methods. Note that all public "sort" methods take the
             * same form: performing argument checks if necessary, and then
             * expanding arguments into those required for the internal
             * implementation methods residing in other package-private
             * classes (except for legacyMergeSort, included in this class).
             */

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(IntArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(IntArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(LongArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(LongArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(ShortArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(ShortArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(CharArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(CharArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(ByteArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(ByteArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all float
             * values: <b> -0.0f == 0.0f</b>  is <b> true</b>  and a <b> Float.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Float#compareTo</b> : <b> -0.0f</b>  is treated as less than value
             * <b> 0.0f</b>  and <b> Float.NaN</b>  is considered greater than any
             * other value and all <b> Float.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(FloatArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all float
             * values: <b> -0.0f == 0.0f</b>  is <b> true</b>  and a <b> Float.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Float#compareTo</b> : <b> -0.0f</b>  is treated as less than value
             * <b> 0.0f</b>  and <b> Float.NaN</b>  is considered greater than any
             * other value and all <b> Float.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(FloatArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all double
             * values: <b> -0.0d == 0.0d</b>  is <b> true</b>  and a <b> Double.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Double#compareTo</b> : <b> -0.0d</b>  is treated as less than value
             * <b> 0.0d</b>  and <b> Double.NaN</b>  is considered greater than any
             * other value and all <b> Double.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(DoubleArray &a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index <b> fromIndex</b> , inclusive, to
             * the index <b> toIndex</b> , exclusive. If <b> fromIndex == toIndex</b> ,
             * the range to be sorted is empty.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all double
             * values: <b> -0.0d == 0.0d</b>  is <b> true</b>  and a <b> Double.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Double#compareTo</b> : <b> -0.0d</b>  is treated as less than value
             * <b> 0.0d</b>  and <b> Double.NaN</b>  is considered greater than any
             * other value and all <b> Double.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort
             * by Vladimir Yaroslavskiy, Jon Bentley, and Joshua Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b> 
             */
            static void sort(DoubleArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(ByteArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(ByteArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(CharArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(CharArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(ShortArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(ShortArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(IntArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(IntArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(LongArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(LongArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all float
             * values: <b> -0.0f == 0.0f</b>  is <b> true</b>  and a <b> Float.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Float#compareTo</b> : <b> -0.0f</b>  is treated as less than value
             * <b> 0.0f</b>  and <b> Float.NaN</b>  is considered greater than any
             * other value and all <b> Float.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(FloatArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all float
             * values: <b> -0.0f == 0.0f</b>  is <b> true</b>  and a <b> Float.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Float#compareTo</b> : <b> -0.0f</b>  is treated as less than value
             * <b> 0.0f</b>  and <b> Float.NaN</b>  is considered greater than any
             * other value and all <b> Float.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(FloatArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all double
             * values: <b> -0.0d == 0.0d</b>  is <b> true</b>  and a <b> Double.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Double#compareTo</b> : <b> -0.0d</b>  is treated as less than value
             * <b> 0.0d</b>  and <b> Double.NaN</b>  is considered greater than any
             * other value and all <b> Double.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void parallelSort(DoubleArray &a);

            /**
             * Sorts the specified range of the array into ascending numerical order.
             * The range to be sorted extends from the index <b> fromIndex</b> ,
             * inclusive, to the index <b> toIndex</b> , exclusive. If
             * <b> fromIndex == toIndex</b> , the range to be sorted is empty.
             *
             * <p>The <b> <</b>  relation does not provide a total order on all double
             * values: <b> -0.0d == 0.0d</b>  is <b> true</b>  and a <b> Double.NaN</b> 
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * <b style="color: orange;"> Double#compareTo</b> : <b> -0.0d</b>  is treated as less than value
             * <b> 0.0d</b>  and <b> Double.NaN</b>  is considered greater than any
             * other value and all <b> Double.NaN</b>  values are considered equal.
             *
             * @implNote The sorting algorithm is a Dual-Pivot Quicksort by
             * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
             * offers O(n log(n)) performance on all data sets, and is typically
             * faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             *
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > a.length</b>
             */
            static void parallelSort(DoubleArray &a, gint fromIndex, gint toIndex);

        private:
            /**
             * The minimum array length below which a parallel sorting
             * algorithm will not further partition the sorting task. Using
             * smaller sizes typically results in memory contention across
             * tasks that makes parallel speedups unlikely.
             */
            static CORE_FAST gint MIN_ARRAY_SORT_GRAN = 1 << 13;

            /**
             * Sorts the specified array of objects into ascending order, according
             * to the <b style="color: green;"> natural ordering</b>  of its elements.
             * All elements in the array must implement the <b style="color: orange;"> Comparable</b> 
             * interface.  Furthermore, all elements in the array must be
             * <i>mutually comparable</i> (that is, <b> e1.compareTo(e2)</b>  must
             * not throw a <b> ClassCastException</b>  for any elements <b> e1</b> 
             * and <b> e2</b>  in the array).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * @implNote The sorting algorithm is a parallel sort-merge that breaks the
             * array into sub-arrays that are themselves sorted and then merged. When
             * the sub-array length reaches a minimum granularity, the sub-array is
             * sorted using the appropriate <b style="color: orange;"> Arrays::sort</b> 
             * method. If the length of the specified array is less than the minimum
             * granularity, then it is sorted using the appropriate <b style="color: orange;"> 
             * Arrays::sort</b>  method. The algorithm requires a
             * working space no greater than the size of the original array. The
             * <b style="color: orange;"> ForkJoin common pool</b>  is used to
             * execute any parallel tasks.
             *
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             *
             * @throws ClassCastException if the array contains elements that are not
             *         <i>mutually comparable</i> (for example, strings and integers)
             * @throws IllegalArgumentException (optional) if the natural
             *         ordering of the array elements is found to violate the
             *         <b style="color: orange;"> Comparable</b>  contract
             */
            template<class T, Class<gbool>::OnlyIf<Class<Comparable<T>>::isSuper<T>()> = true>
            static void sort(Array<T> &a);

            /**
             * Sorts the specified range of the specified array of objects into
             * ascending order, according to the
             * <b style="color: green;"> natural ordering</b>  of its
             * elements.  The range to be sorted extends from index
             * <b> fromIndex</b> , inclusive, to index <b> toIndex</b> , exclusive.
             * (If <b> fromIndex==toIndex</b> , the range to be sorted is empty.)  All
             * elements in this range must implement the <b style="color: orange;"> Comparable</b> 
             * interface.  Furthermore, all elements in this range must be <i>mutually
             * comparable</i> (that is, <b> e1.compareTo(e2)</b>  must not throw a
             * <b> ClassCastException</b>  for any elements <b> e1</b>  and
             * <b> e2</b>  in the array).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * @implNote The sorting algorithm is a parallel sort-merge that breaks the
             * array into sub-arrays that are themselves sorted and then merged. When
             * the sub-array length reaches a minimum granularity, the sub-array is
             * sorted using the appropriate <b style="color: orange;"> Arrays::sort</b> 
             * method. If the length of the specified array is less than the minimum
             * granularity, then it is sorted using the appropriate <b style="color: orange;"> 
             * Arrays::sort</b>  method. The algorithm requires a working
             * space no greater than the size of the specified range of the original
             * array. The <b style="color: orange;"> ForkJoin common pool</b>  is
             * used to execute any parallel tasks.
             *
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param fromIndex the index of the first element (inclusive) to be
             *        sorted
             * @param toIndex the index of the last element (exclusive) to be sorted
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b>  or
             *         (optional) if the natural ordering of the array elements is
             *         found to violate the <b style="color: orange;"> Comparable</b>  contract
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             * @throws ClassCastException if the array contains elements that are
             *         not <i>mutually comparable</i> (for example, strings and
             *         integers).
             */
            template<class T, Class<gbool>::OnlyIf<Class<Comparable<T>>::isSuper<T>()> = true>
            static void parallelSort(Array<T> &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array of objects according to the order induced by
             * the specified comparator.  All elements in the array must be
             * <i>mutually comparable</i> by the specified comparator (that is,
             * <b> c.compare(e1, e2)</b>  must not throw a <b> ClassCastException</b> 
             * for any elements <b> e1</b>  and <b> e2</b>  in the array).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * @implNote The sorting algorithm is a parallel sort-merge that breaks the
             * array into sub-arrays that are themselves sorted and then merged. When
             * the sub-array length reaches a minimum granularity, the sub-array is
             * sorted using the appropriate <b style="color: orange;"> Arrays::sort</b> 
             * method. If the length of the specified array is less than the minimum
             * granularity, then it is sorted using the appropriate <b style="color: orange;"> 
             * Arrays::sort</b>  method. The algorithm requires a
             * working space no greater than the size of the original array. The
             * <b style="color: orange;"> ForkJoin common pool</b>  is used to
             * execute any parallel tasks.
             *
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param cmp the comparator to determine the order of the array.  A
             *        <b> null</b>  value indicates that the elements'
             *        <b style="color: green;"> natural ordering</b>  should be used.
             * @throws ClassCastException if the array contains elements that are
             *         not <i>mutually comparable</i> using the specified comparator
             * @throws IllegalArgumentException (optional) if the comparator is
             *         found to violate the <b style="color: orange;"> Comparator</b>  contract
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static void parallelSort(Array<T> &a, const Comparator<U> &cmp);

            /**
             * Sorts the specified range of the specified array of objects according
             * to the order induced by the specified comparator.  The range to be
             * sorted extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be sorted is empty.)  All elements in the range must be
             * <i>mutually comparable</i> by the specified comparator (that is,
             * <b> c.compare(e1, e2)</b>  must not throw a <b> ClassCastException</b> 
             * for any elements <b> e1</b>  and <b> e2</b>  in the range).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * @implNote The sorting algorithm is a parallel sort-merge that breaks the
             * array into sub-arrays that are themselves sorted and then merged. When
             * the sub-array length reaches a minimum granularity, the sub-array is
             * sorted using the appropriate <b style="color: orange;"> Arrays::sort</b> 
             * method. If the length of the specified array is less than the minimum
             * granularity, then it is sorted using the appropriate <b style="color: orange;"> 
             * Arrays::sort</b>  method. The algorithm requires a working
             * space no greater than the size of the specified range of the original
             * array. The <b style="color: orange;"> ForkJoin common pool</b>  is
             * used to execute any parallel tasks.
             *
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param fromIndex the index of the first element (inclusive) to be
             *        sorted
             * @param toIndex the index of the last element (exclusive) to be sorted
             * @param cmp the comparator to determine the order of the array.  A
             *        <b> null</b>  value indicates that the elements'
             *        <b style="color: green;"> natural ordering</b>  should be used.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b>  or
             *         (optional) if the natural ordering of the array elements is
             *         found to violate the <b style="color: orange;"> Comparable</b>  contract
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             * @throws ClassCastException if the array contains elements that are
             *         not <i>mutually comparable</i> (for example, strings and
             *         integers).
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static void parallelSort(Array<T> &a, const Comparator<U> &cmp, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array of objects into ascending order, according
             * to the <b style="color: green;"> natural ordering</b>  of its elements.
             * All elements in the array must implement the <b style="color: orange;"> Comparable</b> 
             * interface.  Furthermore, all elements in the array must be
             * <i>mutually comparable</i> (that is, <b> e1.compareTo(e2)</b>  must
             * not throw a <b> ClassCastException</b>  for any elements <b> e1</b> 
             * and <b> e2</b>  in the array).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * <p>Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             *
             * <p>The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merging two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             *
             * <p>The implementation was adapted from Tim Peters's list sort for Python
             * (<a href="http://svn.python.org/projects/python/trunk/Objects/listsort.txt">
             * TimSort</a>).  It uses techniques from Peter McIlroy's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             *
             * @param a the array to be sorted
             * @throws ClassCastException if the array contains elements that are not
             *         <i>mutually comparable</i> (for example, strings and integers)
             * @throws IllegalArgumentException (optional) if the natural
             *         ordering of the array elements is found to violate the
             *         <b style="color: orange;"> Comparable</b>  contract
             */
            static void sort(ObjectArray &a);

            /**
             * Sorts the specified range of the specified array of objects into
             * ascending order, according to the
             * <b style="color: green;"> natural ordering</b>  of its
             * elements.  The range to be sorted extends from index
             * <b> fromIndex</b> , inclusive, to index <b> toIndex</b> , exclusive.
             * (If <b> fromIndex==toIndex</b> , the range to be sorted is empty.)  All
             * elements in this range must implement the <b style="color: orange;"> Comparable</b> 
             * interface.  Furthermore, all elements in this range must be <i>mutually
             * comparable</i> (that is, <b> e1.compareTo(e2)</b>  must not throw a
             * <b> ClassCastException</b>  for any elements <b> e1</b>  and
             * <b> e2</b>  in the array).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * <p>Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             *
             * <p>The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merging two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             *
             * <p>The implementation was adapted from Tim Peters's list sort for Python
             * (<a href="http://svn.python.org/projects/python/trunk/Objects/listsort.txt">
             * TimSort</a>).  It uses techniques from Peter McIlroy's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element (inclusive) to be
             *        sorted
             * @param toIndex the index of the last element (exclusive) to be sorted
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b>  or
             *         (optional) if the natural ordering of the array elements is
             *         found to violate the <b style="color: orange;"> Comparable</b>  contract
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             * @throws ClassCastException if the array contains elements that are
             *         not <i>mutually comparable</i> (for example, strings and
             *         integers).
             */
            static void sort(ObjectArray &a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array of objects according to the order induced by
             * the specified comparator.  All elements in the array must be
             * <i>mutually comparable</i> by the specified comparator (that is,
             * <b> c.compare(e1, e2)</b>  must not throw a <b> ClassCastException</b> 
             * for any elements <b> e1</b>  and <b> e2</b>  in the array).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * <p>Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             *
             * <p>The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merging two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             *
             * <p>The implementation was adapted from Tim Peters's list sort for Python
             * (<a href="http://svn.python.org/projects/python/trunk/Objects/listsort.txt">
             * TimSort</a>).  It uses techniques from Peter McIlroy's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             *
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param c the comparator to determine the order of the array.  A
             *        <b> null</b>  value indicates that the elements'
             *        <b style="color: green;"> natural ordering</b>  should be used.
             * @throws ClassCastException if the array contains elements that are
             *         not <i>mutually comparable</i> using the specified comparator
             * @throws IllegalArgumentException (optional) if the comparator is
             *         found to violate the <b style="color: orange;"> Comparator</b>  contract
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static void sort(Array<T> &a, Comparator<U> const &c);

            /**
             * Sorts the specified range of the specified array of objects according
             * to the order induced by the specified comparator.  The range to be
             * sorted extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be sorted is empty.)  All elements in the range must be
             * <i>mutually comparable</i> by the specified comparator (that is,
             * <b> c.compare(e1, e2)</b>  must not throw a <b> ClassCastException</b> 
             * for any elements <b> e1</b>  and <b> e2</b>  in the range).
             *
             * <p>This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             *
             * <p>Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             *
             * <p>The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merging two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             *
             * <p>The implementation was adapted from Tim Peters's list sort for Python
             * (<a href="http://svn.python.org/projects/python/trunk/Objects/listsort.txt">
             * TimSort</a>).  It uses techniques from Peter McIlroy's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             *
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param fromIndex the index of the first element (inclusive) to be
             *        sorted
             * @param toIndex the index of the last element (exclusive) to be sorted
             * @param c the comparator to determine the order of the array.  A
             *        <b> null</b>  value indicates that the elements'
             *        <b style="color: green;"> natural ordering</b>  should be used.
             * @throws ClassCastException if the array contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b>  or
             *         (optional) if the comparator is found to violate the
             *         <b style="color: orange;"> Comparator</b>  contract
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static void sort(Array<T> &a, gint fromIndex, gint toIndex, Comparator<U> const &c);

            /**
             * Cumulates, in parallel, each element of the given array in place,
             * using the supplied function. For example if the array initially
             * holds <b> [2, 1, 0, 3]</b>  and the operation performs addition,
             * then upon return the array holds <b> [2, 3, 3, 6]</b> .
             * Parallel prefix computation is usually more efficient than
             * sequential loops for large arrays.
             *
             * @tparam T the class of the objects in the array
             * @param array the array, which is modified in-place by this method
             * @param op a side-effect-free, associative function to perform the
             * cumulation
             */
            template<class T>
            static void parallelPrefix(Array<T> &array, const BinaryOperator<T> &op);

            /**
             * Performs <b style="color: orange;"> Array::parallelPrefix(ObjectArray, BinaryOperator)</b> 
             * for the given subrange of the array.
             *
             * @tparam T the class of the objects in the array
             * @param array the array
             * @param fromIndex the index of the first element, inclusive
             * @param toIndex the index of the last element, exclusive
             * @param op a side-effect-free, associative function to perform the
             * cumulation
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or <b> toIndex > array.length</b> 
             */
            template<class T>
            static void parallelPrefix(Array<T> &array, gint fromIndex, gint toIndex, const BinaryOperator<T> &op);

            /**
             * Cumulates, in parallel, each element of the given array in place,
             * using the supplied function. For example if the array initially
             * holds <b> [2, 1, 0, 3]</b>  and the operation performs addition,
             * then upon return the array holds <b> [2, 3, 3, 6]</b> .
             * Parallel prefix computation is usually more efficient than
             * sequential loops for large arrays.
             *
             * @param array the array, which is modified in-place by this method
             * @param op a side-effect-free, associative function to perform the
             * cumulation
             * @throws NullPointerException if the specified array or function is null
             */
            static void parallelPrefix(LongArray &array, const LongBinaryOperator &op);

            /**
             * Performs <b style="color: orange;"> Array::parallelPrefix(LongArray, LongBinaryOperator)</b> 
             * for the given subrange of the array.
             *
             * @param array the array
             * @param fromIndex the index of the first element, inclusive
             * @param toIndex the index of the last element, exclusive
             * @param op a side-effect-free, associative function to perform the
             * cumulation
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException
             *     if <b> fromIndex < 0</b>  or <b> toIndex > array.length</b> 
             */
            static void parallelPrefix(LongArray &array, gint fromIndex, gint toIndex, const LongBinaryOperator &op);

            /**
             * Cumulates, in parallel, each element of the given array in place,
             * using the supplied function. For example if the array initially
             * holds <b> [2.0, 1.0, 0.0, 3.0]</b>  and the operation performs addition,
             * then upon return the array holds <b> [2.0, 3.0, 3.0, 6.0]</b> .
             * Parallel prefix computation is usually more efficient than
             * sequential loops for large arrays.
             *
             * <p> Because floating-point operations may not be strictly associative,
             * the returned result may not be identical to the value that would be
             * obtained if the operation was performed sequentially.
             *
             * @param array the array, which is modified in-place by this method
             * @param op a side-effect-free function to perform the cumulation
             */
            static void parallelPrefix(DoubleArray &array, const DoubleBinaryOperator &op);

            /**
             * Cumulates, in parallel, each element of the given array in place,
             * using the supplied function. For example if the array initially
             * holds <b> [2.0, 1.0, 0.0, 3.0]</b>  and the operation performs addition,
             * then upon return the array holds <b> [2.0, 3.0, 3.0, 6.0]</b> .
             * Parallel prefix computation is usually more efficient than
             * sequential loops for large arrays.
             *
             * <p> Because floating-point operations may not be strictly associative,
             * the returned result may not be identical to the value that would be
             * obtained if the operation was performed sequentially.
             *
             * @param array the array, which is modified in-place by this method
             * @param op a side-effect-free function to perform the cumulation
             */
            static void
            parallelPrefix(DoubleArray &array, gint fromIndex, gint toIndex, const DoubleBinaryOperator &op);

            /**
             * Cumulates, in parallel, each element of the given array in place,
             * using the supplied function. For example if the array initially
             * holds <b> [2, 1, 0, 3]</b>  and the operation performs addition,
             * then upon return the array holds <b> [2, 3, 3, 6]</b> .
             * Parallel prefix computation is usually more efficient than
             * sequential loops for large arrays.
             *
             * @param array the array, which is modified in-place by this method
             * @param op a side-effect-free, associative function to perform the
             * cumulation
             */
            static void parallelPrefix(IntArray &array, const IntBinaryOperator &op);

            /**
             * Performs <b style="color: orange;"> Array::parallelPrefix(IntArray, IntBinaryOperator)</b> 
             * for the given subrange of the array.
             *
             * @param array the array
             * @param fromIndex the index of the first element, inclusive
             * @param toIndex the index of the last element, exclusive
             * @param op a side-effect-free, associative function to perform the
             * cumulation
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or <b> toIndex > array.length</b> 
             */
            static void parallelPrefix(IntArray &array, gint fromIndex, gint toIndex, const IntBinaryOperator &op);

            /**
             * Searches the specified array of longs for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the <b style="color: orange;"> Array::sort(LongArray)</b>  method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(LongArray const &a, glong key);

            /**
             * Searches a range of
             * the specified array of longs for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the <b style="color: orange;"> Array::sort(LongArray, int, int)</b>  method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(LongArray const &a, gint fromIndex, gint toIndex, glong key);

            /**
             * Searches the specified array of ints for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the <b style="color: orange;"> Array::sort(IntArray)</b>  method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(IntArray const &a, gint key);

            /**
             * Searches a range of
             * the specified array of ints for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the <b style="color: orange;"> Array::sort(IntArray, int, int)</b>  method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(IntArray const &a, gint fromIndex, gint toIndex, gint key);

            /**
             * Searches the specified array of shorts for the specified value using
             * the binary search algorithm.  The array must be sorted
             * (as by the <b style="color: orange;"> Array::sort(ShortArray)</b>  method) prior to making this call.  If
             * it is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(ShortArray const &a, gshort key);

            /**
             * Searches a range of
             * the specified array of shorts for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the <b style="color: orange;"> Array::sort(ShortArray, int, int)</b>  method)
             * prior to making this call.  If
             * it is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(ShortArray const &a, gint fromIndex, gint toIndex, gshort key);

            /**
             * Searches the specified array of chars for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the <b style="color: orange;"> Array::sort(CharArray)</b>  method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(CharArray const &a, gchar key);

            /**
             * Searches a range of
             * the specified array of chars for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the <b style="color: orange;"> Array::sort(CharArray, int, int)</b>  method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(CharArray const &a, gint fromIndex, gint toIndex, gchar key);

            /**
             * Searches the specified array of bytes for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the <b style="color: orange;"> Array::sort(ByteArray)</b>  method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(ByteArray const &a, gfloat key);

            /**
             * Searches a range of
             * the specified array of bytes for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the <b style="color: orange;"> Array::sort(ByteArray, int, int)</b>  method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(ByteArray const &a, gint fromIndex, gint toIndex, gbyte key);

            /**
             * Searches the specified array of doubles for the specified value using
             * the binary search algorithm.  The array must be sorted
             * (as by the <b style="color: orange;"> Array::sort(DoubleArray)</b>  method) prior to making this call.
             * If it is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.  This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(DoubleArray const &a, gdouble key);

            /**
             * Searches a range of
             * the specified array of doubles for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the <b style="color: orange;"> Array::sort(DoubleArray, int, int)</b>  method)
             * prior to making this call.
             * If it is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.  This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(DoubleArray const &a, gint fromIndex, gint toIndex, gdouble key);

            /**
             * Searches the specified array of floats for the specified value using
             * the binary search algorithm. The array must be sorted
             * (as by the <b style="color: orange;"> Array::sort(FloatArray)</b>  method) prior to making this call. If
             * it is not sorted, the results are undefined. If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found. This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>. The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key. Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(FloatArray const &a, gfloat key);

            /**
             * Searches a range of
             * the specified array of floats for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the <b style="color: orange;"> Array::sort(FloatArray, int, int)</b>  method)
             * prior to making this call. If
             * it is not sorted, the results are undefined. If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found. This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>. The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key. Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(FloatArray const &a, gint fromIndex, gint toIndex, gbyte key);

            /**
             * Searches the specified array for the specified object using the binary
             * search algorithm. The array must be sorted into ascending order
             * according to the
             * <b style="color: green;"> natural ordering</b> 
             * of its elements (as by the
             * <b style="color: orange;"> Array::sort(ObjectArray)</b>  method) prior to making this call.
             * If it is not sorted, the results are undefined.
             * (If the array contains elements that are not mutually comparable (for
             * example, strings and integers), it <i>cannot</i> be sorted according
             * to the natural ordering of its elements, hence results are undefined.)
             * If the array contains multiple
             * elements equal to the specified object, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the search key is not comparable to the elements of the array.
             */
            static gint binarySearch(ObjectArray const &a, Object const &key);

            /**
             * Searches a range of
             * the specified array for the specified object using the binary
             * search algorithm.
             * The range must be sorted into ascending order
             * according to the
             * <b style="color: green;"> natural ordering</b> 
             * of its elements (as by the
             * <b style="color: orange;"> Array::sort(ObjectArray, int, int)</b>  method) prior to making this
             * call.  If it is not sorted, the results are undefined.
             * (If the range contains elements that are not mutually comparable (for
             * example, strings and integers), it <i>cannot</i> be sorted according
             * to the natural ordering of its elements, hence results are undefined.)
             * If the range contains multiple
             * elements equal to the specified object, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the search key is not comparable to the
             *         elements of the array within the specified range.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            static gint binarySearch(ObjectArray const &a, gint fromIndex, gint toIndex, Object const &key);

            /**
             * Searches the specified array for the specified object using the binary
             * search algorithm.  The array must be sorted into ascending order
             * according to the specified comparator (as by the
             * <b style="color: orange;"> Array::sort(ObjectArray, Comparator) sort(Array&lt;T&gt;, Comparator)</b> 
             * method) prior to making this call.  If it is
             * not sorted, the results are undefined.
             * If the array contains multiple
             * elements equal to the specified object, there is no guarantee which one
             * will be found.
             *
             * @tparam T the class of the objects in the array
             * @param a the array to be searched
             * @param key the value to be searched for
             * @param c the comparator by which the array is ordered.  A
             *        <b> null</b>  value indicates that the elements'
             *        <b style="color: green;"> natural ordering</b>  should be used.
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or <b> a.length</b>  if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the array contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator,
             *         or the search key is not comparable to the
             *         elements of the array using this comparator.
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gint binarySearch(Array<T> const &a, T const &key, const Comparator<U> &c);

            /**
             * Searches a range of
             * the specified array for the specified object using the binary
             * search algorithm.
             * The range must be sorted into ascending order
             * according to the specified comparator (as by the
             * <b style="color: orange;"> Array::sort(ObjectArray, int, int, Comparator)
             * sort(Array&lt;T&gt;, int, int, Comparator)</b> 
             * method) prior to making this call.
             * If it is not sorted, the results are undefined.
             * If the range contains multiple elements equal to the specified object,
             * there is no guarantee which one will be found.
             *
             * @tparam T the class of the objects in the array
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @param c the comparator by which the array is ordered.  A
             *        <b> null</b>  value indicates that the elements'
             *        <b style="color: green;"> natural ordering</b>  should be used.
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or <b> toIndex</b>  if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the range contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator,
             *         or the search key is not comparable to the
             *         elements in the range using this comparator.
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0 or toIndex > a.length</b> 
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gint binarySearch(Array<T> const &a, gint fromIndex, gint toIndex, T const &key,
                                     const Comparator<U> &c);

            /**
             * Returns <b> true</b>  if the two specified arrays of longs are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(LongArray const &a, LongArray const &a2);

            /**
             * Returns true if the two specified arrays of longs, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(LongArray const &a, gint aFromIndex, gint aToIndex,
                                LongArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of ints are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(IntArray const &a, IntArray const &a2);

            /**
             * Returns true if the two specified arrays of ints, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(IntArray const &a, gint aFromIndex, gint aToIndex,
                                IntArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of shorts are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(ShortArray const &a, ShortArray const &a2);

            /**
             * Returns true if the two specified arrays of shorts, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(ShortArray const &a, gint aFromIndex, gint aToIndex,
                                ShortArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of chars are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(CharArray const &a, CharArray const &a2);

            /**
             * Returns true if the two specified arrays of chars, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(CharArray const &a, gint aFromIndex, gint aToIndex,
                                CharArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of bytes are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(ByteArray const &a, ByteArray const &a2);

            /**
             * Returns true if the two specified arrays of bytes, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(ByteArray const &a, gint aFromIndex, gint aToIndex,
                                ByteArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of booleans are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(BooleanArray const &a, BooleanArray const &a2);

            /**
             * Returns true if the two specified arrays of booleans, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(BooleanArray const &a, gint aFromIndex, gint aToIndex,
                                BooleanArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of doubles are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * Two doubles <b> d1</b>  and <b> d2</b>  are considered equal if:
             * <pre>    <b> Double.valueOf(d1).equals(Double.valueOf(d2))</b> </pre>
             * (Unlike the <b> ==</b>  operator, this method considers
             * <b> NaN</b>  equal to itself, and 0.0d unequal to -0.0d.)
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             * @see Double#equals(Object)
             */
            static gbool equals(DoubleArray const &a, DoubleArray const &a2);

            /**
             * Returns true if the two specified arrays of doubles, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two doubles <b> d1</b>  and <b> d2</b>  are considered equal if:
             * <pre>    <b> Double.valueOf(d1).equals(Double.valueOf(d2))</b> </pre>
             * (Unlike the <b> ==</b>  operator, this method considers
             * <b> NaN</b>  equal to itself, and 0.0d unequal to -0.0d.)
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             * @see Double#equals(Object)
             */
            static gbool equals(DoubleArray const &a, gint aFromIndex, gint aToIndex,
                                DoubleArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of floats are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.  Also,
             * two array references are considered equal if both are <b> null</b> .
             *
             * Two floats <b> f1</b>  and <b> f2</b>  are considered equal if:
             * <pre>    <b> Float.valueOf(f1).equals(Float.valueOf(f2))</b> </pre>
             * (Unlike the <b> ==</b>  operator, this method considers
             * <b> NaN</b>  equal to itself, and 0.0f unequal to -0.0f.)
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             * @see Float#equals(Object)
             */
            static gbool equals(FloatArray const &a, FloatArray const &a2);

            /**
             * Returns true if the two specified arrays of floats, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two floats <b> f1</b>  and <b> f2</b>  are considered equal if:
             * <pre>    <b> Float.valueOf(f1).equals(Float.valueOf(f2))</b> </pre>
             * (Unlike the <b> ==</b>  operator, this method considers
             * <b> NaN</b>  equal to itself, and 0.0f unequal to -0.0f.)
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             * @see Float#equals(Object)
             */
            static gbool equals(FloatArray const &a, gint aFromIndex, gint aToIndex,
                                FloatArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of Objects are
             * <i>equal</i> to one another.  The two arrays are considered equal if
             * both arrays contain the same number of elements, and all corresponding
             * pairs of elements in the two arrays are equal.  Two objects <b> e1</b> 
             * and <b> e2</b>  are considered <i>equal</i> if
             * <b> Objects.equals(e1, e2)</b> .
             * In other words, the two arrays are equal if
             * they contain the same elements in the same order.  Also, two array
             * references are considered equal if both are <b> null</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return <b> true</b>  if the two arrays are equal
             */
            static gbool equals(ObjectArray const &a, ObjectArray const &a2);

            /**
             * Returns true if the two specified arrays of Objects, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two objects <b> e1</b>  and <b> e2</b>  are considered <i>equal</i> if
             * <b> Objects.equals(e1, e2)</b> .
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gbool equals(ObjectArray const &a, gint aFromIndex, gint aToIndex,
                                ObjectArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Returns <b> true</b>  if the two specified arrays of Objects are
             * <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if both arrays contain the same number
             * of elements, and all corresponding pairs of elements in the two arrays
             * are equal.  In other words, the two arrays are equal if they contain the
             * same elements in the same order.  Also, two array references are
             * considered equal if both are <b> null</b> .
             *
             * <p>Two objects <b> e1</b>  and <b> e2</b>  are considered <i>equal</i> if,
             * given the specified comparator, <b> cmp.compare(e1, e2) == 0</b> .
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return <b> true</b>  if the two arrays are equal
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gbool equals(Array<T> const &a, Array<T> const &a2, const Comparator<U> &c);

            /**
             * Returns true if the two specified arrays of Objects, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two objects <b> e1</b>  and <b> e2</b>  are considered <i>equal</i> if,
             * given the specified comparator, <b> cmp.compare(e1, e2) == 0</b> .
             *
             * @param a the first array to be tested for equality
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return <b> true</b>  if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gbool equals(Array<T> const &a, gint aFromIndex, gint aToIndex,
                                Array<T> const &b, gint bFromIndex, gint bToIndex, const Comparator<U> &c);

            /**
             * Assigns the specified long value to each element of the specified array
             * of longs.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(LongArray &a, glong val);

            /**
             * Assigns the specified long value to each element of the specified
             * range of the specified array of longs.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(LongArray &a, gint fromIndex, gint toIndex, glong val);

            /**
             * Assigns the specified int value to each element of the specified array
             * of ints.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(IntArray &a, gint val);

            /**
             * Assigns the specified int value to each element of the specified
             * range of the specified array of ints.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(IntArray &a, gint fromIndex, gint toIndex, gint val);

            /**
             * Assigns the specified short value to each element of the specified array
             * of shorts.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(ShortArray &a, gshort val);

            /**
             * Assigns the specified short value to each element of the specified
             * range of the specified array of shorts.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(ShortArray &a, gint fromIndex, gint toIndex, gshort val);

            /**
             * Assigns the specified char value to each element of the specified array
             * of chars.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(CharArray &a, gchar val);

            /**
             * Assigns the specified char value to each element of the specified
             * range of the specified array of chars.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(CharArray &a, gint fromIndex, gint toIndex, gchar val);

            /**
             * Assigns the specified byte value to each element of the specified array
             * of bytes.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(ByteArray &a, gbyte val);

            /**
             * Assigns the specified byte value to each element of the specified
             * range of the specified array of bytes.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(ByteArray &a, gint fromIndex, gint toIndex, gbyte val);

            /**
             * Assigns the specified boolean value to each element of the specified
             * array of booleans.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(BooleanArray &a, gbool val);

            /**
             * Assigns the specified boolean value to each element of the specified
             * range of the specified array of booleans.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(BooleanArray &a, gint fromIndex, gint toIndex, gbool val);

            /**
             * Assigns the specified double value to each element of the specified
             * array of doubles.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(DoubleArray &a, gdouble val);

            /**
             * Assigns the specified double value to each element of the specified
             * range of the specified array of doubles.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(DoubleArray &a, gint fromIndex, gint toIndex, gdouble val);

            /**
             * Assigns the specified float value to each element of the specified array
             * of floats.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(FloatArray &a, gfloat val);

            /**
             * Assigns the specified float value to each element of the specified
             * range of the specified array of floats.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             */
            static void fill(FloatArray &a, gint fromIndex, gint toIndex, gfloat val);

            /**
             * Assigns the specified Object reference to each element of the specified
             * array of Objects.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             * @throws ArrayStoreException if the specified value is not of a
             *         runtime type that can be stored in the specified array
             */
            static void fill(ObjectArray &a, Object &val);

            /**
             * Assigns the specified Object reference to each element of the specified
             * range of the specified array of Objects.  The range to be filled
             * extends from index <b> fromIndex</b> , inclusive, to index
             * <b> toIndex</b> , exclusive.  (If <b> fromIndex==toIndex</b> , the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if <b> fromIndex > toIndex</b> 
             * @throws IndexException if <b> fromIndex < 0</b>  or
             *         <b> toIndex > a.length</b> 
             * @throws ArrayStoreException if the specified value is not of a
             *         runtime type that can be stored in the specified array
             */
            static void fill(ObjectArray &a, gint fromIndex, gint toIndex, Object &val);

            /**
             * Copies the specified array, truncating or padding with nulls (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> null</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             * The resulting array is of exactly the same class as the original array.
             *
             * @tparam T the class of the objects in the array
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with nulls
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            template<class T>
            static Array<T> copyOf(Array<T> const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with nulls (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> null</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             * The resulting array is of the class <b> newType</b> .
             *
             * @tparam U the class of the objects in the original array
             * @tparam T the class of the objects in the returned array
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @param newType the class of the copy to be returned
             * @return a copy of the original array, truncated or padded with nulls
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             * @throws ArrayStoreException if an element copied from
             *     <b> original</b>  is not of a runtime type that can be stored in
             *     an array of class <b> newType</b> 
             */
            template<class T, class U>
            static Array<T> copyOf(Array<U> const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with zeros (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> (byte)0</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with zeros
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static ByteArray copyOf(ByteArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with zeros (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> (short)0</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with zeros
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static ShortArray copyOf(ShortArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with zeros (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> 0</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with zeros
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static IntArray copyOf(IntArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with zeros (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> 0L</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with zeros
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static LongArray copyOf(LongArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with null characters (if necessary)
             * so the copy has the specified length.  For all indices that are valid
             * in both the original array and the copy, the two arrays will contain
             * identical values.  For any indices that are valid in the copy but not
             * the original, the copy will contain <b> '\u005cu0000'</b> .  Such indices
             * will exist if and only if the specified length is greater than that of
             * the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with null characters
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static CharArray copyOf(CharArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with zeros (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> 0f</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with zeros
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static FloatArray copyOf(FloatArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with zeros (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> 0d</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with zeros
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static DoubleArray copyOf(DoubleArray const &original, gint newLength);

            /**
             * Copies the specified array, truncating or padding with <b> false</b>  (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain <b> false</b> .
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             *
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with false elements
             *     to obtain the specified length
             * @throws NegativeArraySizeException if <b> newLength</b>  is negative
             */
            static BooleanArray copyOf(BooleanArray const &original, gint newLength);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> null</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             * <p>
             * The resulting array is of exactly the same class as the original array.
             *
             * @tparam T the class of the objects in the array
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with nulls to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            template<class T>
            static Array<T> copyOfRange(Array<T> const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> null</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             * The resulting array is of the class <b> newType</b> .
             *
             * @tparam U the class of the objects in the original array
             * @tparam T the class of the objects in the returned array
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @param newType the class of the copy to be returned
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with nulls to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             * @throws ArrayStoreException if an element copied from
             *     <b> original</b>  is not of a runtime type that can be stored in
             *     an array of class <b> newType</b> .
             */
            template<class T, class U>
            static Array<T> copyOfRange(Array<U> const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> (byte)0</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with zeros to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static ByteArray copyOfRange(ByteArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> (short)0</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with zeros to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static ShortArray copyOfRange(ShortArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> 0</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with zeros to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static IntArray copyOfRange(IntArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> 0L</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with zeros to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static LongArray copyOfRange(LongArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> '\u005cu0000'</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with null characters to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static CharArray copyOfRange(CharArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> 0f</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with zeros to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static FloatArray copyOfRange(FloatArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> 0d</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with zeros to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static DoubleArray copyOfRange(DoubleArray const &original, gint from, gint to);

            /**
             * Copies the specified range of the specified array into a new array.
             * The initial index of the range (<b> from</b> ) must lie between zero
             * and <b> original.length</b> , inclusive.  The value at
             * <b> original[from]</b>  is placed into the initial element of the copy
             * (unless <b> from == original.length</b>  or <b> from == to</b> ).
             * Values from subsequent elements in the original array are placed into
             * subsequent elements in the copy.  The final index of the range
             * (<b> to</b> ), which must be greater than or equal to <b> from</b> ,
             * may be greater than <b> original.length</b> , in which case
             * <b> false</b>  is placed in all elements of the copy whose index is
             * greater than or equal to <b> original.length - from</b> .  The length
             * of the returned array will be <b> to - from</b> .
             *
             * @param original the array from which a range is to be copied
             * @param from the initial index of the range to be copied, inclusive
             * @param to the final index of the range to be copied, exclusive.
             *     (This index may lie outside the array.)
             * @return a new array containing the specified range from the original array,
             *     truncated or padded with false elements to obtain the required length
             * @throws IndexException if <b> from < 0</b> 
             *     or <b> from > original.length</b> 
             * @throws IllegalArgumentException if <b> from > to</b> 
             */
            static BooleanArray copyOfRange(BooleanArray const &original, gint from, gint to);

            /**
             * Returns a fixed-size list backed by the specified array. Changes made to
             * the array will be visible in the returned list, and changes made to the
             * list will be visible in the array. The returned list is
             * <b style="color: orange;"> Serializable</b>  and implements <b style="color: orange;"> RandomAccess</b> .
             *
             * <p>The returned list implements the optional <b> Collection</b>  methods, except
             * those that would change the size of the returned list. Those methods leave
             * the list unchanged and throw <b style="color: orange;"> UnsupportedOperationException</b> .
             *
             * @apiNote
             * This method acts as bridge between array-based and collection-based
             * APIs, in combination with <b style="color: orange;"> Collection#toArray</b> .
             *
             * <p>This method provides a way to wrap an existing array:
             * <pre><b> 
             *     Array&lt;Integer&gt; numbers = ...
             *     ...
             *     List<Integer> values = Arrays::asList(numbers);
             * </b> </pre>
             *
             * <p>This method also provides a convenient way to create a fixed-size
             * list initialized to contain several elements:
             * <pre><b> 
             *     List<String> stooges = Arrays::asList("Larry", "Moe", "Curly");
             * </b> </pre>
             *
             * <p><em>The list returned by this method is modifiable.</em>
             * To create an unmodifiable list, use
             * <b style="color: orange;"> Collections#unmodifiableList Collections.unmodifiableList</b> 
             * or <a href="List.html#unmodifiable">Unmodifiable Lists</a>.
             *
             * @tparam T the class of the objects in the array
             * @param a the array by which the list will be backed
             * @return a list view of the specified array
             */
            template<class T, class ...Vargs>
            static List<T> &asList(Vargs &&...a);

            /**
             * Returns a fixed-size list backed by the specified array. Changes made to
             * the array will be visible in the returned list, and changes made to the
             * list will be visible in the array. The returned list is
             * <b style="color: orange;"> Serializable</b>  and implements <b style="color: orange;"> RandomAccess</b> .
             *
             * <p>The returned list implements the optional <b> Collection</b>  methods, except
             * those that would change the size of the returned list. Those methods leave
             * the list unchanged and throw <b style="color: orange;"> UnsupportedOperationException</b> .
             *
             * @apiNote
             * This method acts as bridge between array-based and collection-based
             * APIs, in combination with <b style="color: orange;"> Collection#toArray</b> .
             *
             * <p>This method provides a way to wrap an existing array:
             * <pre><b> 
             *     Array&lt;Integer&gt; numbers = ...
             *     ...
             *     List<Integer> values = Arrays::asList(numbers);
             * </b> </pre>
             *
             * <p>This method also provides a convenient way to create a fixed-size
             * list initialized to contain several elements:
             * <pre><b> 
             *     List<String> stooges = Arrays::asList("Larry", "Moe", "Curly");
             * </b> </pre>
             *
             * <p><em>The list returned by this method is modifiable.</em>
             * To create an unmodifiable list, use
             * <b style="color: orange;"> Collections#unmodifiableList Collections.unmodifiableList</b> 
             * or <a href="List.html#unmodifiable">Unmodifiable Lists</a>.
             *
             * @tparam T the class of the objects in the array
             * @param a the array by which the list will be backed
             * @return a list view of the specified array
             */
            template<class T>
            static List<T> &asList(Array<T> const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> long</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Long</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(LongArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two non-null <b> int</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Integer</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(IntArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> short</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Short</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(ShortArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> char</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Character</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(CharArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> byte</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Byte</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(ByteArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> boolean</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Boolean</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(BooleanArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> float</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Float</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(FloatArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two <b> double</b>  arrays <b> a</b>  and <b> b</b> 
             * such that <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is the same value that would be
             * obtained by invoking the <b style="color: orange;"> List#hashCode() hashCode</b> 
             * method on a <b style="color: orange;"> List</b>  containing a sequence of <b style="color: orange;"> Double</b> 
             * instances representing the elements of <b> a</b>  in the same order.
             * If <b> a</b>  is <b> null</b> , this method returns 0.
             *
             * @param a the array whose hash value to compute
             * @return a content-based hash code for <b> a</b> 
             */
            static gint hash(DoubleArray const &a);

            /**
             * Returns a hash code based on the contents of the specified array.  If
             * the array contains other arrays as elements, the hash code is based on
             * their identities rather than their contents.  It is therefore
             * acceptable to invoke this method on an array that contains itself as an
             * element,  either directly or indirectly through one or more levels of
             * arrays.
             *
             * <p>For any two arrays <b> a</b>  and <b> b</b>  such that
             * <b> Arrays::equals(a, b)</b> , it is also the case that
             * <b> Arrays::hashCode(a) == Arrays::hashCode(b)</b> .
             *
             * <p>The value returned by this method is equal to the value that would
             * be returned by <b> Arrays::asList(a).hashCode()</b> , unless <b> a</b> 
             * is <b> null</b> , in which case <b> 0</b>  is returned.
             *
             * @param a the array whose content-based hash code to compute
             * @return a content-based hash code for <b> a</b> 
             * @see #deepHashCode(ObjectArray)
             */
            static gint hash(ObjectArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(long)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(LongArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(int)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(IntArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(short)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(ShortArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(char)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(CharArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements
             * are separated by the characters <b> ", "</b>  (a comma followed
             * by a space).  Elements are converted to strings as by
             * <b> String.valueOf(byte)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b>
             */
            static String toString(ByteArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(boolean)</b> .  Returns <b> "null"</b>  if
             * <b> a</b>  is <b> null</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(BooleanArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(float)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(FloatArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * The string representation consists of a list of the array's elements,
             * enclosed in square brackets (<b> "[]"</b> ).  Adjacent elements are
             * separated by the characters <b> ", "</b>  (a comma followed by a
             * space).  Elements are converted to strings as by
             * <b> String.valueOf(double)</b> .
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             */
            static String toString(DoubleArray const &a);

            /**
             * Returns a string representation of the contents of the specified array.
             * If the array contains other arrays as elements, they are converted to
             * strings by the <b style="color: orange;"> Object#toString</b>  method inherited from
             * <b> Object</b> , which describes their <i>identities</i> rather than
             * their contents.
             *
             * <p>The value returned by this method is equal to the value that would
             * be returned by <b> Arrays::asList(a).toString()</b> , unless <b> a</b> 
             * is <b> null</b> , in which case <b> "null"</b>  is returned.
             *
             * @param a the array whose string representation to return
             * @return a string representation of <b> a</b> 
             * @see #deepToString(ObjectArray)
             */
            static String toString(ObjectArray const &a);


            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, using a generator function to compute
             * each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .forEach(i -> array[i] = generator.apply(i));
             * </b> </pre>
             *
             * @tparam T type of elements of the array
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            template<class T>
            static void setAll(Array<T> &array, IntFunction<T> const &generator);

            /**
             * Set all elements of the specified array, in parallel, using the
             * provided generator function to compute each element.
             *
             * <p>If the generator function throws an exception, an unchecked exception
             * is thrown from <b> parallelSetAll</b>  and the array is left in an
             * indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, in parallel, using a generator function
             * to compute each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .parallel()
             *          .forEach(i -> array[i] = generator.apply(i));
             * </b> </pre>
             *
             * @tparam T type of elements of the array
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            template<class T>
            static void parallelSetAll(Array<T> &array, IntFunction<T> const &generator);

            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, using a generator function to compute
             * each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .forEach(i -> array[i] = generator.applyAsInt(i));
             * </b> </pre>
             *
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void setAll(IntArray &array, IntUnaryOperator const &generator);

            /**
             * Set all elements of the specified array, in parallel, using the
             * provided generator function to compute each element.
             *
             * <p>If the generator function throws an exception, an unchecked exception
             * is thrown from <b> parallelSetAll</b>  and the array is left in an
             * indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, in parallel, using a generator function
             * to compute each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .parallel()
             *          .forEach(i -> array[i] = generator.applyAsInt(i));
             * </b> </pre>
             *
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             * value for that position
             */
            static void parallelSetAll(IntArray &array, IntUnaryOperator const &generator);

            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, using a generator function to compute
             * each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .forEach(i -> array[i] = generator.applyAsLong(i));
             * </b> </pre>
             *
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void setAll(LongArray &array, IntToLongFunction const &generator);

            /**
             * Set all elements of the specified array, in parallel, using the
             * provided generator function to compute each element.
             *
             * <p>If the generator function throws an exception, an unchecked exception
             * is thrown from <b> parallelSetAll</b>  and the array is left in an
             * indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, in parallel, using a generator function
             * to compute each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .parallel()
             *          .forEach(i -> array[i] = generator.applyAsLong(i));
             * </b> </pre>
             *
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void parallelSetAll(LongArray &array, IntToLongFunction const &generator);

            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, using a generator function to compute
             * each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .forEach(i -> array[i] = generator.applyAsDouble(i));
             * </b> </pre>
             *
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void setAll(DoubleArray &array, IntToDoubleFunction const &generator);

            /**
             * Set all elements of the specified array, in parallel, using the
             * provided generator function to compute each element.
             *
             * <p>If the generator function throws an exception, an unchecked exception
             * is thrown from <b> parallelSetAll</b>  and the array is left in an
             * indeterminate state.
             *
             * @apiNote
             * Setting a subrange of an array, in parallel, using a generator function
             * to compute each element, can be written as follows:
             * <pre><b> 
             * IntStream.range(startInclusive, endExclusive)
             *          .parallel()
             *          .forEach(i -> array[i] = generator.applyAsDouble(i));
             * </b> </pre>
             *
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void parallelSetAll(DoubleArray &array, IntToDoubleFunction const &generator);

            /**
             * Compares two <b> boolean</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Boolean#compare(boolean, boolean)</b> , at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(BooleanArray, BooleanArray)</b>  for the definition of a
             * common and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(BooleanArray, BooleanArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Boolean.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(BooleanArray const &a, BooleanArray const &b);

            /**
             * Compares two <b> boolean</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Boolean#compare(boolean, boolean)</b> , at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(BooleanArray, int, int, BooleanArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(BooleanArray, int, int, BooleanArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Boolean.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(BooleanArray const &a, gint aFromIndex, gint aToIndex,
                                BooleanArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> byte</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Byte#compare(byte, byte)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(ByteArray, ByteArray)</b>  for the definition of a common and
             * proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(ByteArray, ByteArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Byte.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(ByteArray const &a, ByteArray const &b);

            /**
             * Compares two <b> byte</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Byte#compare(byte, byte)</b> , at a relative index
             * within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(ByteArray, int, int, ByteArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(ByteArray, int, int, ByteArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Byte.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(ByteArray const &a, gint aFromIndex, gint aToIndex,
                                ByteArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> byte</b>  arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Byte#compareUnsigned(byte, byte)</b> , at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(ByteArray, ByteArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Byte.compareUnsigned(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(ByteArray const &a, ByteArray const &b);


            /**
             * Compares two <b> byte</b>  arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Byte#compareUnsigned(byte, byte)</b> , at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(ByteArray, int, int, ByteArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Byte.compareUnsigned(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compareUnsigned(ByteArray const &a, gint aFromIndex, gint aToIndex,
                                        ByteArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> short</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Short#compare(short, short)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(ShortArray, ShortArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(ShortArray, ShortArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Short.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(ShortArray const &a, ShortArray const &b);

            /**
             * Compares two <b> short</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Short#compare(short, short)</b> , at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(ShortArray, int, int, ShortArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(ShortArray, int, int, ShortArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Short.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(ShortArray const &a, gint aFromIndex, gint aToIndex,
                                ShortArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> short</b>  arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Short#compareUnsigned(short, short)</b> , at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(ShortArray, ShortArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Short.compareUnsigned(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(ShortArray const &a, ShortArray const &b);

            /**
             * Compares two <b> short</b>  arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Short#compareUnsigned(short, short)</b> , at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(ShortArray, int, int, ShortArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Short.compareUnsigned(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compareUnsigned(ShortArray const &a, gint aFromIndex, gint aToIndex,
                                        ShortArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> char</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Character#compare(char, char)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(CharArray, CharArray)</b>  for the definition of a common and
             * proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(CharArray, CharArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Character.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(CharArray const &a, CharArray const &b);

            /**
             * Compares two <b> char</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Character#compare(char, char)</b> , at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(CharArray, int, int, CharArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(CharArray, int, int, CharArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Character.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(CharArray const &a, gint aFromIndex, gint aToIndex,
                                CharArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> int</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Integer#compare(int, int)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(IntArray, IntArray)</b>  for the definition of a common and
             * proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(IntArray, IntArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Integer.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(IntArray const &a, IntArray const &b);

            /**
             * Compares two <b> int</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Integer#compare(int, int)</b> , at a relative index
             * within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(IntArray, int, int, IntArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(IntArray, int, int, IntArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Integer.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(IntArray const &a, gint aFromIndex, gint aToIndex,
                                IntArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> int</b>  arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Integer#compareUnsigned(int, int)</b> , at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(IntArray, IntArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Integer.compareUnsigned(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(IntArray const &a, IntArray const &b);

            /**
             * Compares two <b> int</b>  arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Integer#compareUnsigned(int, int)</b> , at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(IntArray, int, int, IntArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Integer.compareUnsigned(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compareUnsigned(IntArray const &a, gint aFromIndex, gint aToIndex,
                                        IntArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> long</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Long#compare(long, long)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(LongArray, LongArray)</b>  for the definition of a common and
             * proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(LongArray, LongArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Long.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(LongArray const &a, LongArray const &b);

            /**
             * Compares two <b> long</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Long#compare(long, long)</b> , at a relative index
             * within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(LongArray, int, int, LongArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(LongArray, int, int, LongArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Long.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(LongArray const &a, gint aFromIndex, gint aToIndex,
                                LongArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> long</b>  arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Long#compareUnsigned(long, long)</b> , at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(LongArray, LongArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Long.compareUnsigned(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(LongArray const &a, LongArray const &b);

            /**
             * Compares two <b> long</b>  arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Long#compareUnsigned(long, long)</b> , at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(LongArray, int, int, LongArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Long.compareUnsigned(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compareUnsigned(LongArray const &a, gint aFromIndex, gint aToIndex,
                                        LongArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> float</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Float#compare(float, float)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(FloatArray, FloatArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(FloatArray, FloatArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Float.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(FloatArray const &a, FloatArray const &b);

            /**
             * Compares two <b> float</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Float#compare(float, float)</b> , at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(FloatArray, int, int, FloatArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(FloatArray, int, int, FloatArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Float.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(FloatArray const &a, gint aFromIndex, gint aToIndex,
                                FloatArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> double</b>  arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b style="color: orange;"> Double#compare(double, double)</b> , at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(DoubleArray, DoubleArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(DoubleArray, DoubleArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return Double.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(DoubleArray const &a, DoubleArray const &b);

            /**
             * Compares two <b> double</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b style="color: orange;"> Double#compare(double, double)</b> , at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(DoubleArray, int, int, DoubleArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(DoubleArray, int, int, DoubleArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if:
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return Double.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint compare(DoubleArray const &a, gint aFromIndex, gint aToIndex,
                                DoubleArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> Object</b>  arrays, within comparable elements,
             * lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements of type <b> T</b>  at
             * an index <b> i</b>  within the respective arrays that is the prefix
             * length, as if by:
             * <pre><b> 
             *     Comparator.nullsFirst(Comparator.<T>naturalOrder()).
             *         compare(a[i], b[i])
             * </b> </pre>
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(ObjectArray, ObjectArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference. Two <b> null</b>  array
             * references are considered equal.
             * A <b> null</b>  array element is considered lexicographically less than a
             * non-<b> null</b>  array element. Two <b> null</b>  array elements are
             * considered equal.
             *
             * <p>The comparison is consistent with <b style="color: orange;"> Array::equals(ObjectArray, ObjectArray) equals</b> ,
             * more specifically the following holds for arrays <b> a</b>  and <b> b</b> :
             * <pre><b> 
             *     Arrays::equals(a, b) == (Arrays::compare(a, b) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references
             * and elements):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return a[i].compareTo(b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @tparam T the type of comparable array elements
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            template<class T, Class<gbool>::OnlyIf<Class<Comparable<T>>::template isSuper<T>()> = true>
            static gint compare(Array<T> const &a, Array<T> const &b);

            /**
             * Compares two <b> Object</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements of type <b> T</b>  at a relative index <b> i</b>  within the
             * respective arrays that is the prefix length, as if by:
             * <pre><b> 
             *     Comparator.nullsFirst(Comparator.<T>naturalOrder()).
             *         compare(a[aFromIndex + i, b[bFromIndex + i])
             * </b> </pre>
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(ObjectArray, int, int, ObjectArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * <b style="color: orange;"> Array::equals(ObjectArray, int, int, ObjectArray, int, int) equals</b> , more
             * specifically the following holds for arrays <b> a</b>  and <b> b</b>  with
             * specified ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively:
             * <pre><b> 
             *     Arrays::equals(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) ==
             *         (Arrays::compare(a, aFromIndex, aToIndex, b, bFromIndex, bToIndex) == 0)
             * </b> </pre>
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array elements):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return a[aFromIndex + i].compareTo(b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @tparam T the type of comparable array elements
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            template<class T, Class<gbool>::OnlyIf<Class<Comparable<T>>::template isSuper<T>()> = true>
            static gint compare(Array<T> const &a, gint aFromIndex, gint aToIndex,
                                Array<T> const &b, gint bFromIndex, gint bToIndex);

            /**
             * Compares two <b> Object</b>  arrays lexicographically using a specified
             * comparator.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing with the specified comparator two
             * elements at an index within the respective arrays that is the prefix
             * length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b style="color: orange;"> Array::mismatch(ObjectArray, ObjectArray)</b>  for the definition of a common
             * and proper prefix.)
             *
             * <p>A <b> null</b>  array reference is considered lexicographically less
             * than a non-<b> null</b>  array reference.  Two <b> null</b>  array
             * references are considered equal.
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array references):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, b, cmp);
             *     if (i >= 0 && i < Math.min(a.length, b.length))
             *         return cmp.compare(a[i], b[i]);
             *     return a.length - b.length;
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the value <b> 0</b>  if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than <b> 0</b>  if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if the first array is
             *         lexicographically greater than the second array
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gint compare(Array<T> const &a, Array<T> const &b, Comparator<U> const &cmp);

            /**
             * Compares two <b> Object</b>  arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing with the
             * specified comparator two elements at a relative index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b style="color: orange;"> Array::mismatch(ObjectArray, int, int, ObjectArray, int, int)</b>  for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if (for non-<b> null</b>  array elements):
             * <pre><b> 
             *     int i = Arrays::mismatch(a, aFromIndex, aToIndex,
             *                             b, bFromIndex, bToIndex, cmp);
             *     if (i >= 0 && i < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             *         return cmp.compare(a[aFromIndex + i], b[bFromIndex + i]);
             *     return (aToIndex - aFromIndex) - (bToIndex - bFromIndex);
             * </b> </pre>
             *
             * @param a the first array to compare
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be compared
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the value <b> 0</b>  if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than <b> 0</b>  if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gint compare(Array<T> const &a, gint aFromIndex, gint aToIndex,
                                Array<T> const &b, gint bFromIndex, gint bToIndex, Comparator<U> const &cmp);

            /**
             * Finds and returns the index of the first mismatch between two
             * <b> boolean</b>  arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(BooleanArray const &a, BooleanArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> boolean</b>  arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     a[aFromIndex + pl] != b[bFromIndex + pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(BooleanArray const &a, gint aFromIndex, gint aToIndex,
                                  BooleanArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two <b> byte</b> 
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(ByteArray const &a, ByteArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> byte</b>  arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     a[aFromIndex + pl] != b[bFromIndex + pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(ByteArray const &a, gint aFromIndex, gint aToIndex,
                                  ByteArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two <b> char</b> 
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(CharArray const &a, CharArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> char</b>  arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     a[aFromIndex + pl] != b[bFromIndex + pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(CharArray const &a, gint aFromIndex, gint aToIndex,
                                  CharArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two <b> short</b> 
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(ShortArray const &a, ShortArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> short</b>  arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     a[aFromIndex + pl] != b[bFromIndex + pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(ShortArray const &a, gint aFromIndex, gint aToIndex,
                                  ShortArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two <b> int</b> 
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(IntArray const &a, IntArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> int</b>  arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     a[aFromIndex + pl] != b[bFromIndex + pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(IntArray const &a, gint aFromIndex, gint aToIndex,
                                  IntArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two <b> long</b> 
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(LongArray const &a, LongArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> long</b>  arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     a[aFromIndex + pl] != b[bFromIndex + pl]
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(LongArray const &a, gint aFromIndex, gint aToIndex,
                                  LongArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two <b> float</b> 
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     Float.compare(a[pl], b[pl]) != 0
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(FloatArray const &a, FloatArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> float</b>  arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     Float.compare(a[aFromIndex + pl], b[bFromIndex + pl]) != 0
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(FloatArray const &a, gint aFromIndex, gint aToIndex,
                                  FloatArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two
             * <b> double</b>  arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     Double.compare(a[pl], b[pl]) != 0
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(DoubleArray const &a, DoubleArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> double</b>  arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     Double.compare(a[aFromIndex + pl], b[bFromIndex + pl]) != 0
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(DoubleArray const &a, gint aFromIndex, gint aToIndex,
                                  DoubleArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two
             * <b> Object</b>  arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl) &&
             *     !Objects.equals(a[pl], b[pl])
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            static gint missmatch(ObjectArray const &a, ObjectArray const &b);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> Object</b>  arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl) &&
             *     !Objects.equals(a[aFromIndex + pl], b[bFromIndex + pl])
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex))
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            static gint missmatch(ObjectArray const &a, gint aFromIndex, gint aToIndex,
                                  ObjectArray const &b, gint bFromIndex, gint bToIndex);

            /**
             * Finds and returns the index of the first mismatch between two
             * <b> Object</b>  arrays, otherwise return -1 if no mismatch is found.
             * The index will be in the range of 0 (inclusive) up to the length
             * (inclusive) of the smaller array.
             *
             * <p>The specified comparator is used to determine if two array elements
             * from the each array are not equal.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(a.length, b.length) &&
             *     Arrays::equals(a, 0, pl, b, 0, pl, cmp)
             *     cmp.compare(a[pl], b[pl]) != 0
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b> , share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     a.length != b.length &&
             *     Arrays::equals(a, 0, Math.min(a.length, b.length),
             *                   b, 0, Math.min(a.length, b.length),
             *                   cmp)
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the index of the first mismatch between the two arrays,
             *         otherwise <b> -1</b> .
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gint mismatch(Array<T> const& a, Array<T> const& b, Comparator<T> const& cmp);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * <b> Object</b>  arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a common
             * prefix of length <b> pl</b>  if the following expression is true:
             * <pre><b> 
             *     pl >= 0 &&
             *     pl < Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex) &&
             *     Arrays::equals(a, aFromIndex, aFromIndex + pl, b, bFromIndex, bFromIndex + pl, cmp) &&
             *     cmp.compare(a[aFromIndex + pl], b[bFromIndex + pl]) != 0
             * </b> </pre>
             * Note that a common prefix length of <b> 0</b>  indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two non-<b> null</b>  arrays, <b> a</b>  and <b> b</b>  with specified
             * ranges [<b> aFromIndex</b> , <b> atoIndex</b> ) and
             * [<b> bFromIndex</b> , <b> btoIndex</b> ) respectively, share a proper
             * prefix if the following expression is true:
             * <pre><b> 
             *     (aToIndex - aFromIndex) != (bToIndex - bFromIndex) &&
             *     Arrays::equals(a, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   b, 0, Math.min(aToIndex - aFromIndex, bToIndex - bFromIndex),
             *                   cmp)
             * </b> </pre>
             *
             * @param a the first array to be tested for a mismatch
             * @param aFromIndex the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aToIndex the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFromIndex the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bToIndex the index (exclusive) of the last element in the
             *                 second array to be tested
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise <b> -1</b> .
             * @throws IllegalArgumentException
             *         if <b> aFromIndex > aToIndex</b>  or
             *         if <b> bFromIndex > bToIndex</b> 
             * @throws IndexException
             *         if <b> aFromIndex < 0 or aToIndex > a.length</b>  or
             *         if <b> bFromIndex < 0 or bToIndex > b.length</b> 
             */
            template<class T, class U, Class<gbool>::OnlyIf<Class<U>::template isSuper<T>()> = true>
            static gint mismatch(Array<T> const& a, gint aFromIndex, gint aToIndex,
                                 Array<T> const& b, gint bFromIndex, gint bToIndex, Comparator<T> const& cmp);
        };

    } // util
} // core

#endif //CORE23_ARRAYS_H
