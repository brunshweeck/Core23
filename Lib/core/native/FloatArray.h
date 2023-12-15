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
        class FloatArray : public Array<Float> {
        private:
            /**
             * gfloat[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gfloat>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

            gbool isLocal = false;

            friend util::ArraysSupport;
            friend native::Unsafe;

            template<class T>
            CORE_ALIAS(CaptureArray, , typename Class<T>::template iff<Class<T>::isArray() &&
                    Class<typename Class<T>::NoArray>::template isAssignable<gfloat>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<gfloat>()>);

        public:

            /**
             * Construct new empty Float Array
             */
            FloatArray() : FloatArray(0) {}

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

            /**
             * Construct new FloatArray instance with address
             *
             * @code
             *  gbool b[50] = {...}
             *  FloatArray ba = FloatArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static FloatArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static array
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<gfloat>()> = true>
            static FloatArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                FloatArray ba(size, (gfloat) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (gfloat) array[i];
                }
                return ba;
            }

            /**
             * Construct new FloatArray list of value
             */
            template<class ...T, Class<gbool>::template Iff<Class<gfloat>::allIsTrue
                    (Class<T>::template isAssignable<gfloat>()...)> = true>
            static FloatArray of(T &&...a) {
                gint size = sizeof...(a);
                FloatArray ba(size);
                for (int i = 0; i < size; ++i) {
                    ba[i] = Class<gfloat>::valueExactAt(i + 1, 0.0F, (gfloat) a...);
                }
                return ba;
            }
        };

    } // core
} // native

#endif //CORE23_FLOATARRAY_H
