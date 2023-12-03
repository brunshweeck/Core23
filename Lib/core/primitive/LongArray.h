//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_LONGARRAY_H
#define CORE23_LONGARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The LongArray class wrap the static array of values from primitive type
         * (generic) glong in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as LongBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class LongArray: public Array<Long> {
        private:
            /**
             * glong[*]
             */
            CORE_ALIAS(STORAGE, typename Class<glong>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Long Array
             */
            LongArray(): LongArray(0) {}

            /**
             * Construct new LongArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
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
             *          The value used to initialize all items after array creation
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
            ~LongArray() override;
        };

    } // core
} // primitive

#endif //CORE23_LONGARRAY_H
