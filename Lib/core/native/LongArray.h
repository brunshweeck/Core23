//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_LONGARRAY_H
#define CORE23_LONGARRAY_H

#include "Array.h"

namespace core {
    namespace native {

        /**
         * The LongArray class wrap the static array of values from native type
         * (generic) glong in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as LongBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class LongArray : public Array<Long> {
        private:
            /**
             * glong[*]
             */
            CORE_ALIAS(STORAGE, typename Class<glong>::Ptr);

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
                    Class<typename Class<T>::NoArray>::template isAssignable<glong>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<glong>()>);

        public:

            /**
             * Construct new empty Long Array
             */
            CORE_FAST LongArray() = default;

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
            glong get(gint index) const override;

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

            /**
             * Construct new LongArray instance with address
             *
             * @code
             *  glong b[50] = {...}
             *  LongArray ba = LongArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static LongArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static address
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<glong>()> = true>
            static LongArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                LongArray ba(size, (glong) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (glong) array[i];
                }
                return ba;
            }

            /**
             * Construct new LongArray list of value
             */
            /*template<class ...T, Class<gbool>::template Iff<Class<glong>::allIsTrue
                    (Class<T>::template isAssignable<glong>()...)> = true>
            static LongArray of(T &&...a) {
                gint size = sizeof...(a);
                LongArray ba(size);
                for (gint i = 0; i < size; ++i) {
                    ba[i] = Class<glong>::valueExactAt(i + 1, 0L, (glong) a...);
                }
                return ba;
            }*/
        };

    } // core
} // native

#endif //CORE23_LONGARRAY_H
