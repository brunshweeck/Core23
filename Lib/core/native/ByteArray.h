//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_BYTEARRAY_H
#define CORE23_BYTEARRAY_H

#include "Array.h"

namespace core {
    namespace native {

        /**
         * The ByteArray class wrap the static array of values from native type
         * (generic) gbyte in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as ByteBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class ByteArray : public Array<Byte> {
        private:
            /**
             * gbyte[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gbyte>::Ptr);

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
                    Class<typename Class<T>::NoArray>::template isAssignable<gbyte>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<gbyte>()>);

        public:

            /**
             * Construct new empty Byte Array
             */
            ByteArray() : ByteArray(0) {}

            /**
             * Construct new ByteArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT ByteArray(gint length);

            /**
             * Construct new ByteArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT ByteArray(gint length, gbyte initialValue);

            /**
             * Initialize newly created ByteArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ByteArray(const ByteArray &array);

            /**
             * Initialize newly created ByteArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ByteArray(ByteArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            ByteArray &operator=(const ByteArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            ByteArray &operator=(ByteArray &&array) CORE_NOTHROW;

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
            gbyte &get(gint index) override;


            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            gbyte get(gint index) const override;

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
            ~ByteArray() override;

            /**
             * Construct new ByteArray instance with address
             *
             * @code
             *  gbyte b[50] = {...}
             *  ByteArray ba = ByteArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static ByteArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static address
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<gbyte>()> = true>
            static ByteArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                ByteArray ba(size, (gbyte) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (gbyte) array[i];
                }
                return ba;
            }

            /**
             * Construct new ByteArray list of value
             */
            /*template<class ...T, Class<gbool>::template Iff<Class<gbyte>::allIsTrue
                    (Class<T>::template isAssignable<gbyte>()...)> = true>
            static ByteArray of(T &&...a) {
                gint size = sizeof...(a);
                ByteArray ba(size);
                for (gint i = 0; i < size; ++i) {
                    ba[i] = Class<gbyte>::valueExactAt(i + 1, 0, (gbyte) a...);
                }
                return ba;
            }*/
        };

    } // core
} // native

#endif //CORE23_BYTEARRAY_H
