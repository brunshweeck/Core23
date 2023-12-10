//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_FLOATARRAY_H
#define CORE23_FLOATARRAY_H

#include "Array.h"

namespace core {
    namespace native {

        /**
         * The FloatArray class wrap the static array of values from native type
         * (generic) gfloat in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as FloatBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class FloatArray: public Array<Float> {
        private:
            /**
             * gfloat[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gfloat>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Float Array
             */
            FloatArray(): FloatArray(0) {}

            /**
             * Construct new FloatArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT FloatArray(gint length);

            /**
             * Construct new FloatArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT FloatArray(gint length, gfloat initialValue);

            /**
             * Initialize newly created FloatArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            FloatArray(const FloatArray &array);

            /**
             * Initialize newly created FloatArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            FloatArray(FloatArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            FloatArray &operator=(const FloatArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            FloatArray &operator=(FloatArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gfloat &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gfloat get(gint index) const override;

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
            ~FloatArray() override;
        };

    } // core
} // native

#endif //CORE23_FLOATARRAY_H
