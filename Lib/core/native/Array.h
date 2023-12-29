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
        class Unsafe;
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

        private:
            CORE_STATIC_ASSERT(Class<Object>::template isSuper<E>(), "Template is not permitted");
            CORE_STATIC_ASSERT(!Class<E>::isConstant(), "Template is not permitted");
            CORE_STATIC_ASSERT(!Class<E>::isReference(), "Template is not permitted");
            CORE_STATIC_ASSERT(!Class<E>::isVolatile(), "Template is not permitted");

            CORE_ALIAS(Es, typename Class<E>::Primitive);
            CORE_ALIAS(V, , Es &);
            CORE_ALIAS(Vc, , typename Class<Es>::template IfElse<Class<Es>::isPrimitive(), const E &>);

        public:

            /**
             * Return number of element in This array
             */
            virtual gint length() const = 0;

            /**
             * Test if this array has no element
             */
            inline gbool isEmpty() const { return length() == 0; }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual V get(gint index) = 0;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            virtual Vc get(gint index) const = 0;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            V operator[](gint index) { return get(index); }

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            Vc operator[](gint index) const { return get(index); }

            /**
             * Return true if specified array has same elements
             * with this array.
             *
             * @param o The other array.
             */
            gbool equals(const Object &o) const override {
                if (!Class<Array>::hasInstance(o))
                    return false;
                Array const &a = (Array &) o;
                gint size1 = length();
                gint size2 = a.length();
                if (size1 != size2)
                    return false;
                for (gint i = 0; i < size1; ++i) {
                    if (get(i) != a.get(i))
                        return false;
                }
                return true;
            }

        private:
            /**
             * The each operations
             */
            template<class T>
            class Itr : public Object {
            private:
                Array &array;
                gint cursor;

            public:
                /**
                 * Construct new Native iterator instance
                 */
                CORE_EXPLICIT Itr(Array &array, gbool fromStart) :
                        array(array), cursor(fromStart ? 0 : array.length()) {}

                Itr &operator++() { return *this; }

                T operator*() { return array[cursor++]; }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Itr>::hasInstance(o))
                        return false;
                    Itr const &it = (Itr &) o;
                    return (&it.array == &array) && (it.cursor == cursor);
                }
            };

        public:
            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            Itr<V> begin() { return Itr<V>(*this, true); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            Itr<Vc> begin() const { return Itr<Vc>((Array &) *this, true); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            Itr<V> end() { return Itr<V>(*this, false); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            Itr<Vc> end() const { return Itr<Vc>((Array &) *this, false); }
        };

    } // core
} // native

#endif //CORE23_ARRAY_H
