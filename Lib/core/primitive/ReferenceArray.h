//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_REFERENCEARRAY_H
#define CORE23_REFERENCEARRAY_H

#include <core/Object.h>
#include <core/private/Unsafe.h>
#include <core/StringBuffer.h>
#include <core/private/Preconditions.h>
#include <core/StateException.h>
#include <core/CastException.h>
#include <core/ArgumentException.h>
#include <core/IndexException.h>
#include "Array.h"

namespace core {
    namespace primitive {

        /**
         * The ReferenceArray class wrap the static array of values from primitive type
         * (generic) object in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * <p>
         * The class can be used as view for all buffer using this primitive type
         * (such as ObjectBuffer)
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        template<class E>
        class ReferenceArray : public Array<Object> {
            CORE_STATIC_ASSERT(Class<Object>::isSuper<E>(), "Illegal template type");
            CORE_STATIC_ASSERT(!Class<E>::isConstant(), "Illegal template type");
            CORE_STATIC_ASSERT(!Class<E>::isReference(), "Illegal template type");

        private:
            CORE_ALIAS(REFERENCE, typename Class<E>::Ptr);
            CORE_ALIAS(STORAGE, typename Class<REFERENCE>::Ptr);

            /**
             * The array used to store references
             */
            STORAGE value;

            /**
             * The number of reference supported by this array
             */
            gint len;

            native::Unsafe &unsafe() {
                return native::Unsafe::U;
            }

        public:
            /**
             * Construct new ReferenceArray with the given number of place
             *
             * @param length The number of place on this array
             * @throws ArgumentException If the given length is negative
             */
            CORE_EXPLICIT ReferenceArray(gint length) {
                if (length < 0)
                    ArgumentException("Negative array length").throws(__trace("core.ReferenceArray"));
                value = unsafe().allocateMemory(1LL * length * unsafe().ARRAY_REFERENCE_INDEX_SCALE);
                len = length;
                for (int i = 0; i < length; ++i) value[i] = null;
            }


            /**
             * Initialize newly created BooleanArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ReferenceArray(const ReferenceArray &array) {
                value = unsafe().allocateMemory(1LL * (len = array.len) * unsafe().ARRAY_REFERENCE_INDEX_SCALE);
                for (int i = 0; i < len; ++i)
                    value[i] = !FORCE_COPY_ON_SET ? array.value[i] :
                               array.isSet(i) ? &unsafe().copyInstance(array[i]) : null;
            }


            /**
             * Initialize newly created BooleanArray with items of another.
             *
             * @param array
             *          The array that items are used to initialize this array
             */
            ReferenceArray(ReferenceArray &&array) CORE_NOTHROW {
                STORAGE oldValue = value;
                gint oldLength = len;
                value = array.value;
                len = array.len;
                array.value = oldValue;
                array.len = oldLength;
            }

            ReferenceArray &operator=(const ReferenceArray &array) {
                if (this != &array) {
                    if (len != array.len)
                        value = unsafe().reallocateMemory(
                                (glong) value,
                                1LL * (len = array.len) * unsafe().ARRAY_REFERENCE_INDEX_SCALE);
                    for (int i = 0; i < len; ++i)
                        if (value[i] != array.value[i])
                            value[i] = !FORCE_COPY_ON_SET ? array.value[i] :
                                       array.isSet(i) ? &unsafe().copyInstance(array[i]) : null;
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
                for (int i = 0; i < len; ++i) {
                    if (!isSet(i) && array.isSet(i))
                        return false;
                    if (!Class<E>::hasInstance(array[i]))
                        return false;
                    if (get(i) != array[i])
                        return false;
                }
                return true;
            }

            Object &clone() const override {
                return unsafe().template createInstance<ReferenceArray>(*this);
            }

            String toString() const override {
                if (isEmpty()) return "[]";
                StringBuffer sb = StringBuffer('[').append(get(0));
                for (int i = 1; i < len; ++i) sb.append(',').append(' ').append(get(i));
                return sb.append("]").toString();
            }

            gint length() const override {
                return len;
            }

            Object &get(gint index) override {
                try {
                    util::Preconditions::checkIndex(index, len);
                } catch (const IndexException &ie) {
                    ie.throws(__trace("core.ReferenceArray"));
                }
                if (!isSet(index))
                    StateException("Null Reference").throws(__trace("core.ReferenceArray"));
                return unsafe().getReference(unsafe().getAddress((glong) value));
            }

            void set(gint index, const Object &newValue) override {
                try {
                    util::Preconditions::checkIndex(index, len);
                } catch (const IndexException &ie) {
                    ie.throws(__trace("core.ReferenceArray"));
                }
                if (!Class<E>::hasInstance(newValue))
                    CastException("Array type not support element of type " + newValue.classname())
                            .throws(__trace("core.ReferenceArray"));
                value[index] = !FORCE_COPY_ON_SET ? &(E &) newValue : &unsafe().copyInstance(newValue);
            }

            void set(gint index, const E &newValue) override {
                try {
                    util::Preconditions::checkIndex(index, len);
                } catch (const IndexException &ie) {
                    ie.throws(__trace("core.ReferenceArray"));
                }
                value[index] = !FORCE_COPY_ON_SET ? &(E &) newValue : &unsafe().copyInstance(newValue);
            }

            gbool isSet(gint index) const override {
                return index >= 0 && index < len && value[index] != null;
            }

            void unset(gint index) override {
                try {
                    util::Preconditions::checkIndex(index, len);
                } catch (const IndexException &ie) {
                    ie.throws(__trace("core.ReferenceArray"));
                }
                value[index] = null;
            }

            ~ReferenceArray() override {
                len = 0;
                unsafe().freeMemory((glong) value);
                value = 0;
            }

            /**
             * Convert this array to reference array of sub-class or super-class
             */
            template<class T, Class<gbool>::
            Iff<Class<T>::template isSuper<E>() || Class<E>::template isSuper<T>()> = true>
            CORE_EXPLICIT operator ReferenceArray() const {}

            /**
             * This property is true if the reference must be copied before place it
             * on this array
             */
            static gbool FORCE_COPY_ON_SET;

        private:

            /**
             * The each operation
             */
            class Itr : public Object {
            private:
                ReferenceArray &root;
                gint next = 0;

                CORE_FAST CORE_EXPLICIT Itr(ReferenceArray &a, gint start) : root(a), next(start) {}

                friend ReferenceArray;

            public:
                CORE_FAST CORE_EXPLICIT Itr(ReferenceArray &a) : root(a) {}

                Itr &operator++() {
                    next += 1;
                    return *this;
                }

                Object &operator*() {
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
                const ReferenceArray &root;
                gint next = 0;

                CORE_FAST CORE_EXPLICIT Itr2(const ReferenceArray &a, gint start) : root(a), next(start) {}

                friend ReferenceArray;

            public:
                CORE_FAST CORE_EXPLICIT Itr2(const ReferenceArray &a) : root(a) {}

                Itr2 &operator++() {
                    next += 1;
                    return *this;
                }

                const Object& operator*() {
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

        template<class E>
        gbool ReferenceArray<E>::FORCE_COPY_ON_SET = true;

    } // core
} // primitive

#endif //CORE23_REFERENCEARRAY_H
