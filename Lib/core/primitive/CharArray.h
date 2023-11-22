//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_CHARARRAY_H
#define CORE23_CHARARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        class CharArray: public Array<Character> {
        private:

            /**
             * The address used to store all chars of this array
             */
            glong addr = 0;

        public:

            /**
             * Construct new empty array
             */
            CORE_FAST CharArray(): Array<Character>(0) {}

            /**
             * Construct new CharArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             *
             * @throws ArgumentException If length is negative.
             */
            CORE_EXPLICIT CharArray(gint length);

            /**
             * Construct new CharArray with specified number
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
            CORE_EXPLICIT CharArray(gint length, gchar initialValue);

            /**
             * Initialize newly created CharArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            CharArray(const CharArray &array);

            /**
             * Initialize newly created CharArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            CharArray(CharArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            CharArray &operator=(const CharArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            CharArray &operator=(CharArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gchar &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gchar get(gint index) const override;

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
            ~CharArray();
        };

    } // core
} // primitive

#endif //CORE23_CHARARRAY_H
