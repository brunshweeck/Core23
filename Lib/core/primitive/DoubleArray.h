//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_DOUBLEARRAY_H
#define CORE23_DOUBLEARRAY_H

#include <core/Double.h>
#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The DoubleArray class wrap the static array of values from primitive type
         * (generic) gdouble in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as DoubleBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class DoubleArray: public Array<Double> {
        private:
            /**
             * gdouble[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gdouble>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Double Array
             */
            DoubleArray(): DoubleArray(0) {}

            /**
             * Construct new DoubleArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT DoubleArray(gint length);

            /**
             * Construct new DoubleArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT DoubleArray(gint length, gdouble initialValue);

            /**
             * Initialize newly created DoubleArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            DoubleArray(const DoubleArray &array);

            /**
             * Initialize newly created DoubleArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            DoubleArray(DoubleArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            DoubleArray &operator=(const DoubleArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            DoubleArray &operator=(DoubleArray &&array) CORE_NOTHROW;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gdouble &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const gdouble get(gint index) const override;

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
            ~DoubleArray() override;
        };

    } // core
} // primitive

#endif //CORE23_DOUBLEARRAY_H
