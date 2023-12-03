//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_BYTEARRAY_H
#define CORE23_BYTEARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The ByteArray class wrap the static array of values from primitive type
         * (generic) gbyte in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as ByteBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class ByteArray: public Array<Byte> {
        private:
            /**
             * gbyte[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gbyte>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Byte Array
             */
            ByteArray(): ByteArray(0) {}

            /**
             * Construct new ByteArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
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
             *          The value used to initialize all items after array creation
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
            ~ByteArray() override;
        };

    } // core
} // primitive

#endif //CORE23_BYTEARRAY_H
