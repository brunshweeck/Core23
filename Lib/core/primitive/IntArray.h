//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_INTARRAY_H
#define CORE23_INTARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The IntArray class wrap the static array of values from primitive type
         * (generic) gint in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as IntBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class IntArray: public Array<Integer> {
        private:
            /**
             * gint[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gint>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new IntArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT IntArray(gint length);

            /**
             * Construct new IntArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT IntArray(gint length, gint initialValue);

            /**
             * Initialize newly created IntArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            IntArray(const IntArray &array);

            /**
             * Initialize newly created IntArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            IntArray(IntArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            IntArray &operator=(const IntArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            IntArray &operator=(IntArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gint &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gint get(gint index) const override;

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
            ~IntArray() override;
        };

    } // core
} // primitive

#endif //CORE23_INTARRAY_H
