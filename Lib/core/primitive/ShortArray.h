//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_SHORTARRAY_H
#define CORE23_SHORTARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The ShortArray class wrap the static array of values from primitive type
         * (generic) gshort in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as ShortBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class ShortArray: public Array<Short> {
        private:
            /**
             * gshort[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gshort>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Short Array
             */
            ShortArray(): ShortArray(0) {}

            /**
             * Construct new ShortArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
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
             *          The value used to initialize all items after array creation
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
            ~ShortArray() override;
        };

    } // core
} // primitive

#endif //CORE23_SHORTARRAY_H
