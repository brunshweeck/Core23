//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_REFERENCEARRAY_H
#define CORE23_REFERENCEARRAY_H

#include <core/private/Unsafe.h>
#include <core/StringBuffer.h>
#include <core/util/Preconditions.h>
#include <core/StateException.h>
#include <core/CastException.h>
#include <core/ArgumentException.h>
#include <core/IndexException.h>
#include "Array.h"

namespace core {

    namespace util {
        template<class> class ArrayList;
        template<class> class Vector;
        template<class> class PriorityQueue;
        template<class,class> class HashMap;
        template<class,class> class HashTable;
    }

    namespace native {

        /**
         * The ReferenceArray class wrap the static array of values from native type
         * (generic) object in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this native type
         * (such as ReferenceBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        template<class E>
        class ReferenceArray : public Array<Object> {
        private:

            CORE_ALIAS(REFERENCE, typename Class<E>::Ptr);

            CORE_ALIAS(STORAGE, typename Class<REFERENCE>::Ptr);

            CORE_ALIAS(U, native::Unsafe);

            /**
             * Capture<T> represent all type T that extends this List element type E.
             * in other word E is base of T (Class<E>::isSuper<T>() is true).
             */
            template<class T>
            CORE_ALIAS(Capture, typename Class<T>::template Iff<Class<E>::template isSuper<T>()>);

            /**
             * The array used to store references
             */
            STORAGE value;

            /**
             * The number of reference supported by this array
             */
            gint len;

            /**
             *  (transient) to simplify nested class access
             */
            template<class T> friend class util::ArrayList;
            template<class T> friend class util::Vector;
            template<class T> friend class util::PriorityQueue;
            template<class K, class V> friend class util::HashMap;
            template<class K, class V> friend class util::HashTable;
            friend native::Unsafe;

        public:

            /**
             * Construct new empty Reference Array
             */
            CORE_FAST ReferenceArray() : value(null), len(0) {}

            /**
             * Construct new ReferenceArray with the given number of place
             *
             * @param length The number of place on this array
             * @throws ArgumentException If the given length is negative
             */
            CORE_EXPLICIT ReferenceArray(gint length) {
                if (length < 0)
                    ArgumentException("Negative array length").throws(__trace("core.native.ReferenceArray"));
                value = (STORAGE) U::allocateMemory(1LL * length * U::ARRAY_REFERENCE_INDEX_SCALE);
                len = length;
                for (gint i = 0; i < length; ++i) value[i] = null;
            }

            /**
             * Initialize newly created ReferenceArray with items of another.
             *
             * @param a The array that items are used to initialize this array
             */
            template<class T>
            CORE_EXPLICIT ReferenceArray(const ReferenceArray<Capture<T>> &a) {
                gint aSize = a.length();
                if ((len = aSize) == 0)
                    value = null;
                else {
                    value = (STORAGE) U::allocateMemory(1LL * aSize * U::ARRAY_REFERENCE_INDEX_SCALE);
                    for (gint i = 0; i < len; ++i) value[i] = a.value[i];
                }
            }

            /**
             * Initialize newly created ReferenceArray with items of another.
             *
             * @param a The array that items are used to initialize this array
             */
            ReferenceArray(const ReferenceArray &a) {
                gint aSize = a.length();
                if ((len = aSize) == 0)
                    value = null;
                else {
                    value = (STORAGE) U::allocateMemory(1LL * aSize * U::ARRAY_REFERENCE_INDEX_SCALE);
                    for (gint i = 0; i < len; ++i) value[i] = a.value[i];
                }
            }


            /**
             * Initialize newly created ReferenceArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ReferenceArray(ReferenceArray &&a) CORE_NOTHROW {
                STORAGE oldValue = value;
                gint oldLength = len;
                value = a.value;
                len = a.len;
                a.value = oldValue;
                a.len = oldLength;
            }

            ReferenceArray &operator=(const ReferenceArray &array) {
                if (this != &array) {
                    if (len != array.len)
                        value = (STORAGE) U::reallocateMemory((glong) value, 1LL * (len = array.len) *
                                                                               U::ARRAY_REFERENCE_INDEX_SCALE);
                    for (gint i = 0; i < len; ++i) value[i] = array.value[i];
                }
            }

            ReferenceArray &operator=(ReferenceArray &&array) CORE_NOTHROW {
                STORAGE oldValue = value;
                gint oldLength = len;
                value = array.value;
                len = array.len;
                array.value = oldValue;
                array.len = oldLength;
                return *this;
            }

            gbool equals(const Object &o) const override {
                if (!Class<Array<Object>>::hasInstance(o))
                    return false;
                Array<Object> const &array = CORE_CAST(const Array<Object>&, o);
                if (len != array.length())
                    return false;
                for (gint i = 0; i < len; ++i) {
                    if (!isSet(i) && array.isSet(i))
                        return false;
                    if (!Class<E>::hasInstance(array[i]))
                        return false;
                    if (get(i) != array[i])
                        return false;
                }
                return true;
            }

            Object &clone() const override { return U::template createInstance<ReferenceArray>(*this); }

            String toString() const override {
                if (isEmpty()) return "[]";
                StringBuffer sb = StringBuffer('[').append(get(0));
                for (gint i = 1; i < len; ++i)
                    sb.append(',').append(' ').append(isSet(i) ? (const Object &) get(i) : null);
                return sb.append("]").toString();
            }

            gint length() const override { return len; }

            E &get(gint index) override {
                try {
                    util::Preconditions::checkIndex(index, len);
                } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
                if (!isSet(index)) StateException("Null Reference").throws(__trace("core.native.ReferenceArray"));
                return *value[index];
            }

            const E &get(gint index) const override {
                try {
                    util::Preconditions::checkIndex(index, len);
                } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
                if (!isSet(index)) StateException("Null Reference").throws(__trace("core.native.ReferenceArray"));
                return *value[index];
            }

            void set(gint index, const E &newValue) {
                try {
                    util::Preconditions::checkIndex(index, len);
                    value[index] = &U::copyInstance(newValue, true);
                } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
            }

            gbool isSet(gint index) const override { return index >= 0 && index < len && value[index] != null; }

            void unset(gint index) override {
                try {
                    util::Preconditions::checkIndex(index, len);
                    value[index] = null;
                } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
            }

            ~ReferenceArray() override {
                len = 0;
                U::freeMemory((glong) value);
                value = null;
            }

            inline E &operator[](gint index) override { return get(index); }

            inline const E &operator[](gint index) const override { return get(index); }

        private:
            /**
             * The each operations
             */
            template<class T>
            class NativeArrayIterator : public Object {
            private:
                ReferenceArray<E> &root;
                gint cursor;

            public:
                /**
                 * Construct new Native iterator instance
                 */
                NativeArrayIterator(ReferenceArray<E> &root, gbool begin) :
                        root(root), cursor(begin ? 0 : root.len) {}

                inline NativeArrayIterator &operator++() { return *this; }

                inline T &operator*() { return root[cursor++]; }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<NativeArrayIterator>::hasInstance(o))
                        return false;
                    NativeArrayIterator &nitr = CORE_CAST(NativeArrayIterator &, o);
                    return (&nitr.root == &root) && ((nitr.cursor == cursor) ||
                                                     ((cursor >= root.len) && (nitr.cursor >= root.len)));
                }
            };

        public:
            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            inline NativeArrayIterator<E> begin() { return NativeArrayIterator<E>(*this, true); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            inline NativeArrayIterator<const E> begin() const {
                return NativeArrayIterator<const E>((ReferenceArray &) *this, true);
            }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            inline NativeArrayIterator<E> end() { return NativeArrayIterator<E>(*this, false); }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            inline NativeArrayIterator<const E> end() const {
                return NativeArrayIterator<const E>((ReferenceArray &) *this, false);
            }


            CORE_STATIC_ASSERT(Class<Object>::template isSuper<E>(),
                               "The valid parameters type must be a class deriving from core.Object");

            CORE_STATIC_ASSERT(!Class<E>::isReference() && !Class<E>::isConstant() && !Class<E>::isVolatile(),
                               "The valid parameters type mustn't have qualifiers (const, volatile, &, &&, ...)");
        };

#if CORE_TEMPLATE_TYPE_DEDUCTION
        ReferenceArray() -> ReferenceArray<Object>;
        ReferenceArray(gint) -> ReferenceArray<Object>;
        template<class T> ReferenceArray(ReferenceArray<T>) -> ReferenceArray<T>;
#endif

    } // core
} // native

#endif //CORE23_REFERENCEARRAY_H
