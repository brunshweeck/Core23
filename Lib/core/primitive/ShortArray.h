//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_SHORTARRAY_H
#define CORE23_SHORTARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        class ShortArray: public Array<Short> {
        private:

            /**
             * The address used to store all shorts of this array
             */
            glong addr = 0;

        public:

            /**
             * Construct new empty array
             */
            CORE_FAST ShortArray(): Array<Short>(0) {}

            /**
             * Construct new ShortArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             *
             * @throws ArgumentException If length is negative.
             */
            CORE_EXPLICIT ShortArray(gint length);

            /**
             * Construct new ShortArray with specified number
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
            CORE_EXPLICIT ShortArray(gint length, gshort initialValue);

            /**
             * Initialize newly created ShortArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ShortArray(const ShortArray &array);

            /**
             * Initialize newly created ShortArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ShortArray(ShortArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            ShortArray &operator=(const ShortArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            ShortArray &operator=(ShortArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gshort &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gshort get(gint index) const override;

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
            ~ShortArray();
        };

    } // core
} // primitive

#endif //CORE23_SHORTARRAY_H
