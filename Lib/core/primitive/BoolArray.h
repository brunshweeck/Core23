//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_BOOLARRAY_H
#define CORE23_BOOLARRAY_H

#include <core/Boolean.h>
#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The BoolArray class wrap the static array of values from primitive type
         * (generic) bool in an object.
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
        class BoolArray: public Array<Boolean> {
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
             * Construct new BoolArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT BoolArray(gint length);

            /**
             * Construct new BoolArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT BoolArray(gint length, gbool initialValue);

            /**
             * Initialize newly created BoolArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            BoolArray(const BoolArray &array);

            /**
             * Initialize newly created BoolArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            BoolArray(BoolArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            BoolArray &operator=(const BoolArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            BoolArray &operator=(BoolArray &&array) CORE_NOTHROW;

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
            ~BoolArray() override;
        };

    } // core
} // primitive

#endif //CORE23_BOOLARRAY_H
