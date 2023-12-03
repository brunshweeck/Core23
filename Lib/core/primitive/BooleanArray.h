//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_BOOLEANARRAY_H
#define CORE23_BOOLEANARRAY_H

#include <core/Boolean.h>
#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The BooleanArray class wrap the static array of values from primitive type
         * (generic) gbool in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as BooleanBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class BooleanArray: public Array<Boolean> {
        private:
            /**
             * gbool[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gbool>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Boolean Array
             */
            BooleanArray(): BooleanArray(0) {}

            /**
             * Construct new BooleanArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT BooleanArray(gint length);

            /**
             * Construct new BooleanArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT BooleanArray(gint length, gbool initialValue);

            /**
             * Initialize newly created BooleanArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            BooleanArray(const BooleanArray &array);

            /**
             * Initialize newly created BooleanArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            BooleanArray(BooleanArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            BooleanArray &operator=(const BooleanArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            BooleanArray &operator=(BooleanArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gbool &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gbool get(gint index) const override;

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
            ~BooleanArray() override;
        };

    } // core
} // primitive

#endif //CORE23_BOOLEANARRAY_H
