//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_LONGARRAY_H
#define CORE23_LONGARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        class LongArray: public Array<Long> {
        private:

            /**
             * The address used to store all longs of this array
             */
            glong addr = 0;

        public:

            /**
             * Construct new empty array
             */
            CORE_FAST LongArray(): Array<Long>(0) {}

            /**
             * Construct new LongArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             *
             * @throws ArgumentException If length is negative.
             */
            CORE_EXPLICIT LongArray(gint length);

            /**
             * Construct new LongArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation.
             *
             * @throws ArgumentException If length is negative.
             */
            CORE_EXPLICIT LongArray(gint length, glong initialValue);

            /**
             * Initialize newly created LongArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            LongArray(const LongArray &array);

            /**
             * Initialize newly created LongArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            LongArray(LongArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            LongArray &operator=(const LongArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            LongArray &operator=(LongArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            glong &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const glong get(gint index) const override;

            /**
             * Return the sharable copy of this object.
             *
             * @throws MemoryError
             *         if memory allocation fail.
             */
            Object &clone() const override;

            /**
             * Destroy this array
             */
            ~LongArray();
        };

    } // core
} // primitive

#endif //CORE23_LONGARRAY_H
