//
// Created by T.N.Brunshweeck on 14/12/2023.
//

#ifndef CORE23_STRINGARRAY_H
#define CORE23_STRINGARRAY_H

#include <core/String.h>
#include <core/native/Array.h>

namespace core {
    namespace native {

        class StringArray CORE_FINAL : public Array<String> {
        private:
            /**
             * gbool[*]
             */
            CORE_ALIAS(STORAGE, typename Class<String>::Ptr);

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
                    Class<typename Class<T>::NoArray>::template isAssignable<String>() >);

            template<class T>
            CORE_ALIAS(Capture, , typename Class<T>::template iff<Class<T>::template isAssignable<String>()>);

        public:

            /**
             * Construct new empty Boolean Array
             */
            CORE_FAST StringArray() = default;

            /**
             * Construct new StringArray with specified number
             * of items. After creation all items value will be
             * initialized with random value.
             *
             * @param length
             *          The number of items
             */
            CORE_EXPLICIT StringArray(gint length);

            /**
             * Construct new StringArray with specified number
             * of items. After creation all items value will be
             * initialized with specified initial value.
             *
             * @param length
             *          The number of items
             * @param initialValue
             *          The value used to initialize all items after array creation
             */
            CORE_EXPLICIT StringArray(gint length, const String &initialValue);

            /**
             * Initialize newly created StringArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            StringArray(const StringArray &array);

            /**
             * Initialize newly created StringArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            StringArray(StringArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are used to set this array items
             */
            StringArray &operator=(const StringArray &array);

            /**
             * Exchange with items of specified array, all items of this array.
             *
             * @param array
             *          The array that items are  exchanged with items of this
             */
            StringArray &operator=(StringArray &&array) CORE_NOTHROW;

            /**
             * Return number of String on this array
             */
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
            String &get(gint index) override;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            const String &get(gint index) const override;

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
            ~StringArray() override;

            /**
             * Construct new StringArray instance with address
             *
             * @code
             *  string b[50] = {...}
             *  StringArray ba = StringArray::fromAddress((glong)b, 50);
             *
             * @endcode
             *
             * @param addr The local address (pointer)
             * @param length The number of value
             */
            static StringArray fromAddress(glong addr, gint length);

            /**
             * Construct new Boolean Array with c static address
             */
            template<class T>
            static StringArray copyOf(T &&array) {
                CORE_STATIC_ASSERT(Class<T>::isArray(), "Argument must be C array type (T[])");
                CORE_STATIC_ASSERT(Class<typename Class<T>::NoArray>::template isAssignable<String>(),
                        "Couldn't assign the given array value to String instance");
                gint size = sizeof(T) / sizeof(typename Class<T>::NoArray);
                if (size == 0)
                    return {};
                StringArray ba(size);
                for (int i = 0; i < size; ++i) {
                    ba[i] = (String) array[i];
                }
                return ba;
            }

            /**
             * Construct new StringArray list of value
             */
            template<class ...T>
            static StringArray of(T &&...a) {
                CORE_STATIC_ASSERT(Class<String>::allIsTrue(Class<T>::template isAssignable<String>()...),
                                   "Couldn't assign argument value to String instance");
                gint size = sizeof...(a);
                if (size == 0)
                    return {};
                StringArray ba(size);
                for (int i = 0; i < size; ++i) {
                    ba[i] = Class<String>::valueExactAt(i+1, String(), (T &&) a...);
                }
                return ba;
            }
        };

    } // native
} // core

#endif //CORE23_STRINGARRAY_H
