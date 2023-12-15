//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_CHARARRAY_H
#define CORE23_CHARARRAY_H

#include "Array.h"

namespace core {
    namespace native {

        /**
         * The CharArray class wrap the static array of values from native type
         * (generic) gchar in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as CharBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class CharArray : public Array<Character> {
        private:
            /**
             * gchar[*]
             */
            CORE_ALIAS(STORAGE, typename Class<gchar>::Ptr);

            /**
             * The items storage
             */
            STORAGE value = null;

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
             * Construct new empty Char Array
             */
            CharArray() : CharArray(0) {}

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

            /**
             * Construct new CharArray instance with address
             *
             * @code
             *  gchar b[50] = {...}
             *  CharArray ba = CharArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static CharArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static address
             */
            template<class T, Class<gbool>::template Iff<
                    Class<T>::isArray() && Class<typename Class<T>::NoArray>::template isAssignable<gchar>()> = true>
            static CharArray copyOf(T &&array) {
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                CharArray ba(size, (gchar) array[0]);
                for (int i = 1; i < size; ++i) {
                    ba[i] = (gchar) array[i];
                }
                return ba;
            }

            /**
             * Construct new CharArray list of value
             */
            template<class ...T, Class<gbool>::template Iff<Class<gchar>::allIsTrue
                    (Class<T>::template isAssignable<gchar>()...)> = true>
            static CharArray of(T &&...a) {
                gint size = sizeof...(a);
                CharArray ba(size);
                for (int i = 0; i < size; ++i) {
                    ba[i] = Class<gbool>::valueExactAt(i + 1, 0, (gchar) a...);
                }
                return ba;
            }
        };

    } // core
} // native

#endif //CORE23_CHARARRAY_H
