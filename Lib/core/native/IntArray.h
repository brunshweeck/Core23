//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_INTARRAY_H
#define CORE23_INTARRAY_H

#include "Array.h"

namespace core {
    namespace native {

        /**
         * The IntArray class wrap the static array of values from native type
         * (generic) gint in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as IntBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class IntArray : public Array<Integer> {
        private:
            /**
             * gint[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gint>::Ptr);

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
                    Class<typename Class<T>::NoArray>::template isAssignable<gint>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<gint>()>);

        public:

            /**
             * Construct new empty Int Array
             */
            CORE_FAST IntArray() = default;

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
            gint get(gint index) const override;

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

            /**
             * Construct new IntArray instance with address
             *
             * @code
             *  gint b[50] = {...}
             *  IntArray ba = IntArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static IntArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static address
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<gint>()> = true>
            static IntArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                IntArray ba(size, (gint) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (gint) array[i];
                }
                return ba;
            }

            /**
             * Construct new IntArray list of value
             */
            /*template<class ...T, Class<gbool>::template Iff<Class<gint>::allIsTrue
                    (Class<T>::template isAssignable<gint>()...)> = true>
            static IntArray of(T &&...a) {
                gint size = sizeof...(a);
                IntArray ba(size);
                for (gint i = 0; i < size; ++i) {
                    ba[i] = Class<gint>::valueExactAt(i + 1, 0, (gint) a...);
                }
                return ba;
            }*/
        };

    } // core
} // native

#endif //CORE23_INTARRAY_H
