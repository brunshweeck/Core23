//
// Created by T.N.Brunshweeck on 24/11/2023.
//

#ifndef CORE23_PRECONDITIONS_H
#define CORE23_PRECONDITIONS_H

#include <core/Object.h>

namespace core {
    namespace util {

        /**
         * Utility methods to check if state or arguments are correct.
         */
        class Preconditions CORE_FINAL: public Object {
        private:
            CORE_FAST Preconditions() = default;

        public:

            /**
             * Checks if the index is within the bounds of the range from 0 (inclusive) to length (exclusive).
             *
             * <p>
             * The index is defined to be out of bounds if any of the following inequalities is true:
             * <ul>
             *  <li> index < 0</li>
             *  <li> index >= length</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param index
             *          the index
             * @param length
             *          the upper-bound (exclusive) of the range
             *
             * @throws IndexException
             *          if the index is out of bounds
             */
            static gint checkIndex(gint index, gint length);

            /**
             * Checks if the sub-range from startIndex (inclusive) to endIndex (exclusive)
             * is within the bounds of range from 0 (inclusive) to length (exclusive).
             *
             * <p>
             * The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li> startIndex < 0</li>
             *  <li> startIndex > endIndex</li>
             *  <li> endIndex > length</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param startIndex
             *          the lower-bound (inclusive) of the sub-range
             * @param endIndex
             *          the upper-bound (exclusive) of the sub-range
             * @param length
             *          the upper-bound (exclusive) the range
             * @throws IndexException
             *          if the sub-range is out of bounds
             */
            static gint checkIndexFromRange(gint startIndex, gint endIndex, gint length);

            /**
             * Checks if the sub-range from startIndex (inclusive) to
             * startIndex + size (exclusive) is within the bounds of range from
             * 0 (inclusive) to length (exclusive).
             *
             * <p>The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li> startIndex < 0</li>
             *  <li> size < 0</li>
             *  <li> startIndex + size > length, taking into account integer overflow</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param startIndex
             *          the lower-bound (inclusive) of the sub-interval
             * @param size
             *          the size of the sub-range
             * @param length
             *          the upper-bound (exclusive) of the range
             * @throws IndexException
             *          if the sub-range is out of bounds
             */
            static gint checkIndexFromSize(gint startIndex, gint size, gint length);

            /**
             * Checks if the index is within the bounds of the range from 0 (inclusive) to length (inclusive).
             *
             * <p>
             * The index is defined to be out of bounds if any of the following inequalities is true:
             * <ul>
             *  <li> index < 0</li>
             *  <li> index > length</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param index
             *          the index
             * @param length
             *          the upper-bound (inclusive) of the range
             *
             * @throws IndexException
             *          if the index is out of bounds
             */
            static gint checkIndexForAdding(gint index, gint length);

            /**
             * Checks if the index is within the bounds of the range from 0 (inclusive) to length (exclusive).
             *
             * <p>
             * The index is defined to be out of bounds if any of the following inequalities is true:
             * <ul>
             *  <li> index < 0</li>
             *  <li> index >= length</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param index
             *          the index
             * @param length
             *          the upper-bound (exclusive) of the range
             *
             * @throws IndexException
             *          if the index is out of bounds
             */
            static glong checkIndex(glong index, glong length);

            /**
             * Checks if the sub-range from startIndex (inclusive) to endIndex (exclusive)
             * is within the bounds of range from 0 (inclusive) to length (exclusive).
             *
             * <p>
             * The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li> startIndex < 0</li>
             *  <li> startIndex > endIndex</li>
             *  <li> endIndex > length</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param startIndex
             *          the lower-bound (inclusive) of the sub-range
             * @param endIndex
             *          the upper-bound (exclusive) of the sub-range
             * @param length
             *          the upper-bound (exclusive) the range
             * @throws IndexException
             *          if the sub-range is out of bounds
             */
            static glong checkIndexFromRange(glong startIndex, glong endIndex, glong length);

            /**
             * Checks if the sub-range from startIndex (inclusive) to
             * startIndex + size (exclusive) is within the bounds of range from
             * 0 (inclusive) to length (exclusive).
             *
             * <p>The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li> startIndex < 0</li>
             *  <li> size < 0</li>
             *  <li> startIndex + size > length, taking into account integer overflow</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param startIndex
             *          the lower-bound (inclusive) of the sub-interval
             * @param size
             *          the size of the sub-range
             * @param length
             *          the upper-bound (exclusive) of the range
             * @throws IndexException
             *          if the sub-range is out of bounds
             */
            static glong checkIndexFromSize(glong startIndex, glong size, glong length);

            /**
             * Checks if the index is within the bounds of the range from 0 (inclusive) to length (inclusive).
             *
             * <p>
             * The index is defined to be out of bounds if any of the following inequalities is true:
             * <ul>
             *  <li> index < 0</li>
             *  <li> index > length</li>
             *  <li> length < 0, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param index
             *          the index
             * @param length
             *          the upper-bound (inclusive) of the range
             *
             * @throws IndexException
             *          if the index is out of bounds
             */
            static glong checkIndexForAdding(glong index, glong length);
        };
    }
} // core

#endif //CORE23_PRECONDITIONS_H
