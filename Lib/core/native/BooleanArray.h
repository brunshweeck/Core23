//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_BOOLEANARRAY_H
#define CORE23_BOOLEANARRAY_H

#include <core/Boolean.h>
#include "Array.h"

namespace core {
    namespace native {

/**
         * The BooleanArray class wrap the static array of values from native type
         * (generic) gbool in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as BooleanBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class BooleanArray : public Array<Boolean> {
        private:
            /**
             * gbool[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gbool>::Ptr);

            gint len = {};

            /**
             * The items storage
             */
            STORAGE value = {};

            gbool isLocal = false;

            friend util::ArraysSupport;
            friend native::Unsafe;

            template<class T>
            CORE_ALIAS(CaptureArray, , typename Class<T>::template iff<Class<T>::isArray() &&
                    Class<typename Class<T>::NoArray>::template isAssignable<gbool>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<gbool>()>);

        public:

            /**
             * Construct new empty Boolean Array
             */
            BooleanArray() : BooleanArray(0) {}

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

            gint length() const override;

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
            gbool get(gint index) const override;

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

            /**
             * Construct new BooleanArray instance with address
             *
             * @code
             *  gbool b[50] = {...}
             *  BooleanArray ba = BooleanArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static BooleanArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static array
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<gbool>()> = true>
            static BooleanArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                BooleanArray ba(size, (gbool) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (gbool) array[i];
                }
                return ba;
            }

            /**
             * Construct new BooleanArray list of value
             */
            /*template<class ...T, Class<gbool>::template Iff<Class<gbool>::allIsTrue
                    (Class<T>::template isAssignable<gbool>()...)> = true>
            static BooleanArray of(T &&...a) {
                gint size = sizeof...(a);
                BooleanArray ba(size);
                for (gint i = 0; i < size; ++i) {
                    ba[i] = Class<gbool>::valueExactAt(i + 1, false, (gbool) a...);
                }
                return ba;
            }*/
        };

    } // core
} // native

#endif //CORE23_BOOLEANARRAY_H
