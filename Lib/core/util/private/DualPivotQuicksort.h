//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#ifndef CORE23_DUALPIVOTQUICKSORT_H
#define CORE23_DUALPIVOTQUICKSORT_H

#include <core/util/Arrays.h>

namespace core {
    namespace util {

        /**
         * This class implements powerful and fully optimized versions, both
         * sequential and parallel, of the Dual-Pivot Quicksort algorithm by
         * Vladimir Yaroslavskiy, Jon Bentley and Josh Bloch. This algorithm
         * offers O(n log(n)) performance on all data sets, and is typically
         * faster than traditional (one-pivot) Quicksort implementations.
         *
         * There are also additional algorithms, invoked from the Dual-Pivot
         * Quicksort, such as mixed insertion sort, merging of runs and heap
         * sort, counting sort and parallel merge sort.
         *
         * @author Vladimir Yaroslavskiy
         * @author Jon Bentley
         * @author Josh Bloch
         * @author Doug Lea
         *
         * @version 2018.08.18
         */
        class DualPivotQuicksort {
        private:
            DualPivotQuicksort() {}

            /**
             * Max array size to use mixed insertion sort.
             */
            static CORE_FAST int MAX_MIXED_INSERTION_SORT_SIZE = 65;

            /**
             * Max array size to use insertion sort.
             */
            static CORE_FAST int MAX_INSERTION_SORT_SIZE = 44;

            /**
             * Min array size to perform sorting in parallel.
             */
            static CORE_FAST int MIN_PARALLEL_SORT_SIZE = 4 << 10;

            /**
             * Min array size to try merging of runs.
             */
            static CORE_FAST int MIN_TRY_MERGE_SIZE = 4 << 10;

            /**
             * Min size of the first run to continue with scanning.
             */
            static CORE_FAST int MIN_FIRST_RUN_SIZE = 16;

            /**
             * Min factor for the first runs to continue scanning.
             */
            static CORE_FAST int MIN_FIRST_RUNS_FACTOR = 7;

            /**
             * Max capacity of the index array for tracking runs.
             */
            static CORE_FAST int MAX_RUN_CAPACITY = 5 << 10;

            /**
             * Min number of runs, required by parallel merging.
             */
            static CORE_FAST int MIN_RUN_COUNT = 4;

            /**
             * Min array size to use parallel merging of parts.
             */
            static CORE_FAST int MIN_PARALLEL_MERGE_PARTS_SIZE = 4 << 10;

            /**
             * Min size of a byte array to use counting sort.
             */
            static CORE_FAST int MIN_BYTE_COUNTING_SORT_SIZE = 64;

            /**
             * Min size of a short or char array to use counting sort.
             */
            static CORE_FAST int MIN_SHORT_OR_CHAR_COUNTING_SORT_SIZE = 1750;

            /**
             * Threshold of mixed insertion sort is incremented by this value.
             */
            static CORE_FAST int DELTA = 3 << 1;

            /**
             * Max recursive partitioning depth before using heap sort.
             */
            static CORE_FAST int MAX_RECURSION_DEPTH = 64 * DELTA;

            /**
             * Calculates the double depth of parallel merging.
             * Depth is negative, if tasks split before sorting.
             *
             * @param parallelism the parallelism level
             * @param size the target size
             * @return the depth of parallel merging
             */
            static gint depth(gint parallelism, gint size);

            /**
             * Sorts the specified range of the array using parallel merge
             * sort and/or Dual-Pivot Quicksort.
             *
             * To balance the faster splitting and parallelism of merge sort
             * with the faster element partitioning of Quicksort, ranges are
             * subdivided in tiers such that, if there is enough parallelism,
             * the four-way parallel merge is started, still ensuring enough
             * parallelism to process the partitions.
             *
             * @param a the array to be sorted
             * @param parallelism the parallelism level
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static gint sort(IntArray &a, gint parallelism, gint low, gint high);

        public:
            class Sorter;

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases, possibly with parallel partitions.
             *
             * @param sorter parallel context
             * @param a the array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(Sorter const &sorter, IntArray &a, gint bits, gint low, gint high);

        private:

            /**
             * Sorts the specified range of the array using mixed insertion sort.
             *
             * Mixed insertion sort is combination of simple insertion sort,
             * pin insertion sort and pair insertion sort.
             *
             * In the context of Dual-Pivot Quicksort, the pivot element
             * from the left part plays the role of sentinel, because it
             * is less than any elements from the given part. Therefore,
             * expensive check of the left range can be skipped on each
             * iteration unless it is the leftmost call.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param end the index of the last element for simple insertion sort
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void mixedInsertionSort(IntArray &a, int low, int end, int high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(IntArray a, int low, int high);

            /**
             * Sorts the specified range of the array using heap sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void heapSort(IntArray &a, int low, int high);

            /**
             * Pushes specified element down during heap sort.
             *
             * @param a the given array
             * @param p the start index
             * @param value the given element
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void pushDown(IntArray &a, int p, int value, int low, int high);

            /**
             * Tries to sort the specified range of the array.
             *
             * @param sorter parallel context
             * @param a the array to be sorted
             * @param low the index of the first element to be sorted
             * @param size the array size
             * @return true if finally sorted, false otherwise
             */
            static gbool tryMergeRuns(Sorter const &sorter, IntArray a, int low, int size);

            /**
             * Merges the specified runs.
             *
             * @param a the source array
             * @param b the temporary buffer used in merging
             * @param offset the start index in the source, inclusive
             * @param aim specifies merging: to source ( > 0), buffer ( < 0) or any ( == 0)
             * @param parallel indicates whether merging is performed in parallel
             * @param run the start indexes of the runs, inclusive
             * @param lo the start index of the first run, inclusive
             * @param hi the start index of the last run, inclusive
             * @return the destination where runs are merged
             */
            static IntArray mergeRuns(IntArray &a, IntArray &b, int offset,
                                      int aim, gbool parallel, IntArray &run, int lo, int hi);

            class Merger;

            /**
             * Merges the sorted parts.
             *
             * @param merger parallel context
             * @param dst the destination where parts are merged
             * @param k the start index of the destination, inclusive
             * @param a1 the first part
             * @param lo1 the start index of the first part, inclusive
             * @param hi1 the end index of the first part, exclusive
             * @param a2 the second part
             * @param lo2 the start index of the second part, inclusive
             * @param hi2 the end index of the second part, exclusive
             */
            static void mergeParts(Merger merger, IntArray dst, int k,
                                   IntArray a1, int lo1, int hi1, IntArray a2, int lo2, int hi2);
        };

    } // util
} // core

#endif //CORE23_DUALPIVOTQUICKSORT_H
