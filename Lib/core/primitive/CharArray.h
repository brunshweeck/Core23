//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_CHARARRAY_H
#define CORE23_CHARARRAY_H

#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The CharArray class wrap the static array of values from primitive type
         * (generic) gchar in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as CharBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class CharArray: public Array<Character> {
        private:
            /**
             * gchar[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gchar>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            friend util::ArraysSupport;

        public:

            /**
             * Construct new empty Char Array
             */
            CharArray(): CharArray(0) {}

            /**
             * Construct new CharArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
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
             *          The value used to initialize all items after array creation
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
            ~CharArray() override;
        };

    } // core
} // primitive

#endif //CORE23_CHARARRAY_H
