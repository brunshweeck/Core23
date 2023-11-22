//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_ARRAY_H
#define CORE23_ARRAY_H

#include <core/Object.h>

namespace core {
    namespace primitive {

        /**
         * The Array class represent the static collection
         * of items that are instance of primitive type
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
        interface Array : public Object {

            CORE_STATIC_ASSERT(
                    Class<E>::template isSimilar<Byte>() || // -> ByteArray
                    Class<E>::template isSimilar<Short>() || // -> ShortArray
                    Class<E>::template isSimilar<Character>() || // -> CharArray
                    Class<E>::template isSimilar<Integer>() || // -> IntArray
                    Class<E>::template isSimilar<Long>() || // LongArray
                    Class<E>::template isSimilar<Object>(), "Template is not permitted");

        private:
            /**
             * The length of array
             */
            gint len = 0;

            CORE_ALIAS(Es, typename Class<E>::Primitive);

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
                (*this)[index] = newValue;
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
                const Array &a = CORE_DYN_CAST(Array &, object);
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

        private:
            /**
             * The each operation
             */
            class Itr : public Object {
            private:
                Array &root;
                gint next = 0;

                CORE_FAST CORE_EXPLICIT Itr(Array &a, gint start) : root(a), next(start) {}

                friend Array;

            public:
                CORE_FAST CORE_EXPLICIT Itr(Array &a) : root(a) {}

                Itr &operator++() {
                    next += 1;
                    return *this;
                }

                Es &operator*() {
                    return root[next - 1];
                }

                gbool equals(const Object &o) const override {
                    if (!Class<Itr>::hasInstance(o))
                        return false;
                    const Itr &itr = CORE_DYN_CAST(Itr &, o);
                    return &root == &itr.root && next == itr.next;
                }
            };

            class Itr2 : public Object {
            private:
                const Array &root;
                gint next = 0;

                CORE_FAST CORE_EXPLICIT Itr2(const Array &a, gint start) : root(a), next(start) {}

                friend Array;

            public:
                CORE_FAST CORE_EXPLICIT Itr2(const Array &a) : root(a) {}

                Itr2 &operator++() {
                    next += 1;
                    return *this;
                }

                const Es operator*() {
                    return root[next - 1];
                }

                gbool equals(const Object &o) const override {
                    if (!Class<Itr>::hasInstance(o))
                        return false;
                    const Itr &itr = CORE_DYN_CAST(Itr &, o);
                    return &root == &itr.root && next == itr.next;
                }
            };

        public:
            Itr begin() {
                return Itr(*this);
            }

            Itr2 begin() const {
                return Itr2(*this);
            }

            Itr end() {
                return Itr(*this, len);
            }

            Itr2 end() const {
                return Itr2(*this, len);
            }
        };

    } // core

    using namespace primitive;
} // primitive

#endif //CORE23_ARRAY_H
