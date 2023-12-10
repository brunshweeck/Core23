//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_ARRAY_H
#define CORE23_ARRAY_H

#include <core/Integer.h>
#include <core/private/Null.h>

namespace core {

    namespace util {
        class ArraysSupport;
    }

    namespace native {

        /**
         * The Array class represent the static collection
         * of items that are instance of native type
         *
         * @param E
         *          The object type of item
         *
         * @see ByteArray
         * @see CharArray
         * @see ShortArray
         * @see IntArray
         * @see LongArray
         */
        template<class E>
        class Array : public Object {

            CORE_STATIC_ASSERT(
                    Class<E>::template isSimilar<Boolean>() || // -> BooleanArray
                    Class<E>::template isSimilar<Byte>() || // -> ByteArray
                    Class<E>::template isSimilar<Short>() || // -> ShortArray
                    Class<E>::template isSimilar<Character>() || // -> CharArray
                    Class<E>::template isSimilar<Integer>() || // -> IntArray
                    Class<E>::template isSimilar<Long>() || // LongArray
                    Class<E>::template isSimilar<Float>() || // LongArray
                    Class<E>::template isSimilar<Double>(), "Template is not permitted");

        private:
            /**
             * The length of array
             */
            gint len = 0;

            CORE_ALIAS(Es, typename Class<E>::Primitive);

            friend BooleanArray;
            friend ByteArray;
            friend ShortArray;
            friend CharArray;
            friend IntArray;
            friend LongArray;
            friend FloatArray;
            friend DoubleArray;

        public:

            /**
             * Construct new Array with specified length
             *
             * @param length The number of element in this Array
             */
            CORE_FAST Array(gint length) : len(length < 0 ? 0 : length) {}

            /**
             * Return number of element in This array
             */
            CORE_FAST gint length() const {
                return len;
            }

            /**
             * Test if this array has no element
             */
            CORE_FAST gbool isEmpty() const {
                return length() == 0;
            }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual Es &get(gint index) = 0;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual const Es get(gint index) const {
                return CORE_CAST(Array &, *this)[index];
            }

            /**
             * Set the value at specified index with specified new value
             *
             * @param index
             *          The Position of desired value
             * @param newValue
             *          The replacement value
             * @throws IndexException
             *          If index out of bounds
             */
            virtual const Es set(gint index, Es newValue) {
                Es es = get(index);
                (*this)[index] = newValue;
                return es;
            }

            /**
             * Return true iff specified object is array instance of same type
             * that has same items as this.
             *
             * @param object
             *          The object to be compared
             */
            gbool equals(const Object &object) const override {
                if (Object::equals(object))
                    return true;
                if (!Class<Array>::hasInstance(object))
                    return false;
                const Array &a = CORE_DYN_CAST(const Array &, object);
                if (len != a.len)
                    return false;
                else
                    for (gint i = 0; i < len; ++i) {
                        if ((*this)[0] != a[i])
                            return false;
                    }
                return true;
            }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual Es &operator[](gint index) {
                return get(index);
            }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual const Es operator[](gint index) const {
                return get(index);
            }

            virtual ~Array() = default;

        private:
            /**
             * The each operations
             */
            template<class T>
            class NativeArrayIterator : public Object {
            private:
                Array<E> &root;
                gint cursor;

            public:
                /**
                 * Construct new Native iterator instance
                 */
                NativeArrayIterator(Array<E> &root, gbool begin) :
                        root(root), cursor(begin ? 0 : root.len) {}

                inline NativeArrayIterator &operator++() { return *this; }

                inline T &operator*() { return root[cursor++]; }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<NativeArrayIterator>::hasInstance(o))
                        return false;
                    NativeArrayIterator &nitr = CORE_CAST(NativeArrayIterator &, o);
                    return (&nitr.root == &root) &&
                           ((nitr.cursor == cursor) || ((cursor >= root.len) && (nitr.cursor >= root.len)));
                }
            };

        public:
            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            NativeArrayIterator<Es> begin() { return NativeArrayIterator<Es>(*this, true); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            NativeArrayIterator<const Es> begin() const { return NativeArrayIterator<const Es>((Array &) *this, true); }
            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            NativeArrayIterator<Es> end() { return NativeArrayIterator<Es>(*this, false); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            NativeArrayIterator<const Es> end() const { return NativeArrayIterator<const Es>((Array &) *this, false); }
        };

        template<>
        class Array<Object> : public Object {
        public:

            /**
             * Return number of element in This array
             */
            virtual gint length() const = 0;

            /**
             * Test if this array has no element
             */
            gbool isEmpty() const {
                return length() == 0;
            }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             *
             * @throws StateException If the value is not set
             */
            virtual Object &get(gint index) = 0;

            /**
             * Return item at specified index iff it exist
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             * @throws StateException If the value is not set
             */
            virtual const Object &get(gint index) const {
                return CORE_CAST(Array<Object> &, *this).get(index);
            }

            /**
             * Return true if the reference at the given index exists (is not null)
             */
            virtual gbool isSet(gint index) const = 0;

            /**
             * mark the reference at the given index like not exist (null)
             */
            virtual void unset(gint index) = 0;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual Object &operator[](gint index) {
                return get(index);
            }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual const Object &operator[](gint index) const {
                return get(index);
            }

            virtual ~Array() = default;

        private:

            /**
             * The each operations
             */
            template<class T>
            class NativeArrayIterator : public Object {
            private:
                Array &root;
                gint cursor;

            public:
                /**
                 * Construct new Native iterator instance
                 */
                NativeArrayIterator(Array &root, gbool begin) :
                        root(root), cursor(begin ? 0 : Integer::MAX_VALUE) {}

                inline NativeArrayIterator &operator++() { return *this; }

                inline T &operator*() {
                    if (!root.isSet(cursor)){
                        cursor++;
                        return null;
                    }
                    return root[cursor++];
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<NativeArrayIterator>::hasInstance(o))
                        return false;
                    NativeArrayIterator &nitr = CORE_CAST(NativeArrayIterator &, o);
                    return (&nitr.root == &root) &&
                           ((nitr.cursor == cursor) || (cursor >= root.length() && (nitr.cursor >= root.length())));
                }
            };

        public:
            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            NativeArrayIterator<Object> begin() { return NativeArrayIterator<Object>(*this, true); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            NativeArrayIterator<const Object> begin() const { return NativeArrayIterator<const Object>((Array &) *this, true); }
            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            NativeArrayIterator<Object> end() { return NativeArrayIterator<Object>(*this, false); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            NativeArrayIterator<const Object> end() const { return NativeArrayIterator<const Object>((Array &) *this, false); }
        };

    } // core
} // native

#endif //CORE23_ARRAY_H
