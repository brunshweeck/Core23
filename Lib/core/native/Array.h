//
// Created by T.N.Brunshweeck on 20/01/2024.
//

#ifndef CORE23_ARRAY_H
#define CORE23_ARRAY_H

#include <core/native/ObjectArray.h>
#include <core/private/Unsafe.h>
#include <core/ClassCastException.h>
#include <core/IndexException.h>

namespace core {
    namespace native {

        /**
         * The reference array (used mainly by collection and map)
         */
        template<class E>
        class Array CORE_FINAL : public ObjectArray {
            // E must be extends Object
        private:
            // gint len;
            // VALUE value;
        public:
            /**
             * Construct empty array
             */
            Array() = default;

            /**
             * @deprecated all value is not initialized with non compatible value (null).
             *              It recommended to call constructor with initial reference
             */
            CORE_EXPLICIT Array(gint length) : ObjectArray(length) {}

            /**
             * Construct new array with specified number of element.
             * all element are initialized with given initial reference
             */
            CORE_EXPLICIT Array(gint length, E &initialValue) : ObjectArray(length, initialValue) {}

            /**
             * Construct new array with specified number of element.
             * all element are initialized with given initial reference
             */
            CORE_EXPLICIT Array(gint length, const E &initialValue) : ObjectArray(length, initialValue) {}

            Array(const Array &array) : ObjectArray(array) {}

            Array(Array &&array) CORE_NOTHROW: ObjectArray((ObjectArray &&) array) {}

            Array &operator=(const Array &array) {
                ((ObjectArray &) *this) = array;
                return *this;
            }

            Array &operator=(Array &&array) CORE_NOTHROW {
                ((ObjectArray &) *this) = (ObjectArray &&) array;
                return *this;
            }

            gint length() const override {
                return ObjectArray::length();
            }

            gbool isEmpty() const override {
                return ObjectArray::isEmpty();
            }

            E &get(gint index) override {
                try {
                    Object &obj = ObjectArray::get(index);
                    return CORE_DYN_CAST(E &, obj);
                } catch (const IndexException &ex) {
                    ex.throws(__trace("core.native.Array"));
                }
            }

            E const &get(gint index) const override {
                try {
                    Object const &obj = ObjectArray::get(index);
                    return CORE_DYN_CAST(E const&, obj);
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.native.Array"));
                }
            }

            void set(gint index, Object &obj) override {
                try {
                    ObjectArray::set(index, CORE_DYN_CAST(E &, obj));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.native.Array"));
                }
            }

            void set(gint index, const Object &obj) override {
                try {
                    ObjectArray::set(index, CORE_DYN_CAST(E const&, obj));
                } catch (const Exception &ex) {
                    ex.throws(__trace("core.native.Array"));
                }
            }

            Object &clone() const override {
                return Unsafe::allocateInstance<Array>(*this);
            }

            ~Array() override = default;


            /**
             * Create new empty array
             */
            static Array of() {
                return {};
            }

            /**
             * Create array initialized with one reference
             */
            static Array of(const E &v0) {
                try {
                    Array array{1};
                    array.set(0, v0);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with two references
             */
            static Array of(const E &v0, const E &v1) {
                try {
                    Array array{2};
                    array.set(0, v0);
                    array.set(1, v1);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with three references
             */
            static Array of(const E &v0, const E &v1, const E &v2) {
                try {
                    Array array{3};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with four references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3) {
                try {
                    Array array{4};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with five references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4) {
                try {
                    Array array{5};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    array.set(4, v4);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with six references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4, const E &v5) {
                try {
                    Array array{6};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    array.set(4, v4);
                    array.set(5, v5);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with seven references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4, const E &v5, const E &v6) {
                try {
                    Array array{7};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    array.set(4, v4);
                    array.set(5, v5);
                    array.set(6, v6);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with eight references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4, const E &v5, const E &v6, const E &v7) {
                try {
                    Array array{8};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    array.set(4, v4);
                    array.set(5, v5);
                    array.set(6, v6);
                    array.set(7, v7);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with nine references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4, const E &v5, const E &v6, const E &v7,
                            const E &v8) {
                try {
                    Array array{9};
                    array.set(0, v0);
                    array.set(1, v1);
                    array.set(2, v2);
                    array.set(3, v3);
                    array.set(4, v4);
                    array.set(5, v5);
                    array.set(6, v6);
                    array.set(7, v7);
                    array.set(8, v8);
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with teen references
             */
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4, const E &v5, const E &v6, const E &v7,
                            const E &v8, const E &v9) {
                try {
                    Array array{10};
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
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            /**
             * Create array initialized with teen and more references
             */
            template<class ...Values>
            static Array of(const E &v0, const E &v1, const E &v2, const E &v3,
                            const E &v4, const E &v5, const E &v6, const E &v7,
                            const E &v8, const E &v9, Values &&...others) {
                CORE_FAST gint n = sizeof...(Values);
                try {
                    Array array{10 + n};
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
                        E const &obj = Class<E const &>::valueExactAt(i + 1, v0,
                                                                      (typename Class<Values &&>::template IfElse<!Class<Values>::isPrimitive(), typename Class<Values>::Object>) others...);
                        array.set(10 + i, obj);
                    }
                    return (Array &&) array;
                } catch (const Throwable &th) {
                    th.throws();
                }
                return {};
            }

            E &operator[](gint index) override {
                return get(index);
            }

            const E &operator[](gint index) const override {
                return get(index);
            }

        protected:
            gbool validateObject(const Object &obj) const override {
                return Class<E>::template isSimilar<Object>() || Class<E>::hasInstance(obj);
            }

        private:
            using typename ObjectArray::CIterator;

        public:
            LinearIterator<E> begin() {
                return LinearIterator<E>(CIterator(*this, 0));
            }

            LinearIterator<E const> begin() const {
                return LinearIterator<E const>(CIterator((ObjectArray &) *this, 0));
            }

            LinearIterator<E> end() {
                return LinearIterator<E>(CIterator(*this, length()));
            }

            LinearIterator<E const> end() const {
                return LinearIterator<E const>(CIterator((ObjectArray &) *this, length()));
            }
        };

    } // native
} // core

#endif //CORE23_ARRAY_H
