//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#ifndef CORE23_OBJECTARRAY_H
#define CORE23_OBJECTARRAY_H

#include <core/MemoryError.h>
#include <core/NoSuchElementException.h>
#include <core/ClassCastException.h>
#include <core/private/Null.h>

namespace core {
    namespace native {

        /**
         * The base class of reference array
         *
         * @see core.util.Array
         */
        class ObjectArray : public Object {
        private:
            // Object[*]
            CORE_ALIAS(REF, typename Class<Object>::Pointer);
            CORE_ALIAS(VALUE, typename Class<REF>::Pointer);

            /**
             * number of object reference on this array
             */
            gint len;

            /**
             * The container of this array
             */
            VALUE value;

            CORE_FRATERNITY(::core::native::Unsafe);
            CORE_FRATERNITY_T1(::core::util::PriorityQueue);

        public:
            /**
             * Construct new empty array
             */
            ObjectArray();

            /**
             * Construct new array with specified length.
             * all value are initialized to null reference
             */
            CORE_EXPLICIT ObjectArray(gint length);

            /**
             * Construct new array with specified length and
             * initialize all value with specified reference
             */
            CORE_EXPLICIT ObjectArray(gint length, Object &initialValue);

            /**
             * Construct new array with specified length and
             * initialize all value with copy of specified reference
             */
            CORE_EXPLICIT ObjectArray(gint length, const Object &initialValue);

            /**
             * Construct new array with other array
             */
            ObjectArray(const ObjectArray &array);

            /**
             * Construct new array with other array (no risk)
             */
            ObjectArray(ObjectArray &&array) CORE_NOTHROW;

            /**
             * Set this array with other array
             */
            ObjectArray &operator=(const ObjectArray &array);

            /**
             * Set this array with other array (no risk)
             */
            ObjectArray &operator=(ObjectArray &&array) CORE_NOTHROW;

            /**
             * Get number of reference on this array
             */
            virtual gint length() const;

            /**
             * Tell if this array has no references
             */
            virtual gbool isEmpty() const;

            /**
             * Get object reference at specified index
             *
             * @throws IndexException If index out of bounds
             */
            virtual Object &get(gint index);

            /**
             * Get object reference at specified index
             *
             * @throws IndexException If index out of bounds
             */
            virtual Object const &get(gint index) const;

            /**
             * Replace object reference at specified index with
             * specified new object reference
             */
            virtual void set(gint index, Object &newValue);

            /**
             * Replace object reference at specified index with
             * copy of specified new object reference
             */
            virtual void set(gint index, const Object &newValue);

            gbool equals(const Object &o) const override;

            Object &clone() const override;

            String toString() const override;

            ~ObjectArray() override = default;

            /**
             * Create new empty array
             */
            static ObjectArray of();

            /**
             * Create array initialized with one reference
             */
            static ObjectArray of(const Object &v0);

            /**
             * Create array initialized with two references
             */
            static ObjectArray of(const Object &v0, const Object &v1);

            /**
             * Create array initialized with three references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2);

            /**
             * Create array initialized with four references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3);

            /**
             * Create array initialized with five references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4);

            /**
             * Create array initialized with six references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4, const Object &v5);

            /**
             * Create array initialized with seven references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4, const Object &v5, const Object &v6);

            /**
             * Create array initialized with eight references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4, const Object &v5, const Object &v6, const Object &v7);

            /**
             * Create array initialized with nine references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4, const Object &v5, const Object &v6, const Object &v7,
                                  const Object &v8);

            /**
             * Create array initialized with teen references
             */
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4, const Object &v5, const Object &v6, const Object &v7,
                                  const Object &v8, const Object &v9);

            /**
             * Create array initialized with teen and more references
             */
            template<class ...Values>
            static ObjectArray of(const Object &v0, const Object &v1, const Object &v2, const Object &v3,
                                  const Object &v4, const Object &v5, const Object &v6, const Object &v7,
                                  const Object &v8, const Object &v9, Values &&...others) {
                CORE_FAST gint n = sizeof...(Values);
                try {
                    ObjectArray array{10 + n};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    array.set(4, v4);
                    array.set(5, v5);
                    array.set(6, v6);
                    array.set(7, v7);
                    array.set(8, v8);
                    array.set(9, v9);
                    for (int i = 0; i < n; i += 1) {
                        Object const &obj = Class<Object const &>::valueExactAt(i + 1, null,
                                                                                (typename Class<Values &&>::template IfElse<!Class<Values>::isPrimitive(), typename Class<Values>::Object>) others...);
                        array.set(10 + i, obj);
                    }
                    return (ObjectArray &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            virtual Object &operator[](gint index);

            virtual Object const &operator[](gint index) const;

        protected:
            /**
             * This method is used by <b> get</b> method to
             * check if object is supported by this array
             */
            virtual gbool validateObject(const Object& obj) const {
                CORE_IGNORE(obj);
                return true;
            }

            class CIterator CORE_FINAL : public Object {
            private:
                ObjectArray &array;
                gint cursor;
                gint limit;

            public:
                CORE_EXPLICIT CIterator(ObjectArray &array, gint cursor);

                gbool hasNext() const;

                Object &next();

                gbool equals(const Object &o) const override;
            };

            template<class T>
            class LinearIterator CORE_FINAL {
            private:
                CIterator base;

            public:
                CORE_EXPLICIT LinearIterator(const CIterator &it) : base(it) {}

                T &operator*() {
                    Object &obj = base.next();
                    if (!Class<T>::hasInstance(obj))
                        ClassCastException("Could not cast object of type " + obj.classname() + " to array element")
                                .throws(__trace("core.native.ObjectArray.LinearIterator"));
                    return CORE_DYN_CAST(T &, obj);
                }

                LinearIterator &operator++() {
                    return *this;
                }

                gbool operator==(const LinearIterator &rhs) const {
                    return base == rhs.base;
                }

                gbool operator!=(const LinearIterator &rhs) const {
                    return !(rhs == *this);
                }
            };

        public:
            LinearIterator<Object> begin() {
                return LinearIterator<Object>(CIterator(*this, 0));
            }

            LinearIterator<Object const> begin() const {
                return LinearIterator<Object const>(CIterator((ObjectArray &) *this, 0));
            }

            LinearIterator<Object> end() {
                return LinearIterator<Object>(CIterator(*this, length()));
            }

            LinearIterator<Object const> end() const {
                return LinearIterator<Object const>(CIterator((ObjectArray &) *this, length()));
            }
        };

    } // native
} // core

#endif //CORE23_OBJECTARRAY_H
