//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_SHORTARRAY_H
#define CORE23_SHORTARRAY_H

#include "Array.h"

namespace core {
    namespace native {

        /**
         * The ShortArray class wrap the static array of values from native type
         * (generic) gshort in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as ShortBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class ShortArray : public Array<Short> {
        private:
            /**
             * gshort[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gshort>::Ptr);

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
                    Class<typename Class<T>::NoArray>::template isAssignable<gshort>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<gshort>()>);

        public:

            /**
             * Construct new empty Short Array
             */
            CORE_FAST ShortArray() = default;

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
            gshort get(gint index) const override;

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

            /**
             * Construct new ShortArray instance with address
             *
             * @code
             *  gshort b[50] = {...}
             *  ShortArray ba = ShortArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static ShortArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static address
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<gshort>()> = true>
            static ShortArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                ShortArray ba(size, (gshort) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (gshort) array[i];
                }
                return ba;
            }

            /**
             * Construct new ShortArray list of value
             */
            /*template<class ...T, Class<gbool>::template Iff<Class<gshort>::allIsTrue
                    (Class<T>::template isAssignable<gshort>()...)> = true>
            static ShortArray of(T &&...a) {
                gint size = sizeof...(a);
                ShortArray ba(size);
                for (gint i = 0; i < size; ++i) {
                    ba[i] = Class<gshort>::valueExactAt(i + 1, (gshort) 0, (gshort) a...);
                }
                return ba;
            }*/
        };

    } // core
} // native

#endif //CORE23_SHORTARRAY_H
