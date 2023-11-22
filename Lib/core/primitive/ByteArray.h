//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_BYTEARRAY_H
#define CORE23_BYTEARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        class ByteArray: public Array<Byte> {
        private:

            /**
             * The address used to store all bytes of this array
             */
            glong addr = 0;

        public:

            /**
             * Construct new empty array
             */
            CORE_FAST ByteArray(): Array<Byte>(0) {}

            /**
             * Construct new ByteArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             *
             * @throws ArgumentException If length is negative.
             */
            CORE_EXPLICIT ByteArray(gint length);

            /**
             * Construct new ByteArray with specified number
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
            CORE_EXPLICIT ByteArray(gint length, gbyte initialValue);

            /**
             * Initialize newly created ByteArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ByteArray(const ByteArray &array);

            /**
             * Initialize newly created ByteArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ByteArray(ByteArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            ByteArray &operator=(const ByteArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            ByteArray &operator=(ByteArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gbyte &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gbyte get(gint index) const override;

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
            ~ByteArray();
        };

    } // core
} // primitive

#endif //CORE23_BYTEARRAY_H
