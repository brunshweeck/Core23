//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#ifndef CORE23_PRIMITIVEARRAY_H
#define CORE23_PRIMITIVEARRAY_H

#include <core/NoSuchElementException.h>

namespace core {

    namespace native {

        /**
         * The Array class represent the static collection
         * of items that are INSTANCE of native type
         *
         * @param E
         *          The object type of item (If E is reference the values are detached from root)
         *
         * @see ByteArray
         * @see CharArray
         * @see ShortArray
         * @see IntArray
         * @see LongArray
         */
        template<class E>
        class PrimitiveArray : public Object {

        private:

            CORE_STATIC_ASSERT(Class<Object>::template isSuper<E>(), "Template is not permitted");
            CORE_STATIC_ASSERT(!Class<E>::isConstant(), "Template is not permitted");
            CORE_STATIC_ASSERT(!Class<E>::isVolatile(), "Template is not permitted");
            CORE_STATIC_ASSERT(
                    Class<E>::template isSimilar<Boolean>()
                    || Class<E>::template isSimilar<Byte>()
                    || Class<E>::template isSimilar<Short>()
                    || Class<E>::template isSimilar<Character>()
                    || Class<E>::template isSimilar<Integer>()
                    || Class<E>::template isSimilar<Long>()
                    || Class<E>::template isSimilar<Float>()
                    || Class<E>::template isSimilar<Double>()
                    || Class<E>::template isSimilar<String>()
                    || Class<E>::template isSimilar<io::File>() // exceptionally for File.childList()
                    || Class<E>::template isSimilar<BooleanArray>()
                    || Class<E>::template isSimilar<ByteArray>()
                    || Class<E>::template isSimilar<ShortArray>()
                    || Class<E>::template isSimilar<CharArray>()
                    || Class<E>::template isSimilar<IntArray>()
                    || Class<E>::template isSimilar<LongArray>()
                    || Class<E>::template isSimilar<FloatArray>()
                    || Class<E>::template isSimilar<DoubleArray>()
                    || Class<E>::template isSimilar<StringArray>(),
                    "Template not represent object form of primitive type");
        protected:
            CORE_ALIAS(Value, , typename Class<E>::Primitive);

        public:
            using Row = typename Class<BooleanArray>::IfElse<
                    Class<E>::template isSimilar<Boolean>(), typename Class<ByteArray>::IfElse<
                            Class<E>::template isSimilar<Byte>(), typename Class<ShortArray>::IfElse<
                                    Class<E>::template isSimilar<Short>(), typename Class<CharArray>::IfElse<
                                            Class<E>::template isSimilar<Character>(), typename Class<IntArray>::IfElse<
                                                    Class<E>::template isSimilar<Integer>(), typename Class<LongArray>::IfElse<
                                                            Class<E>::template isSimilar<Long>(), typename Class<FloatArray>::IfElse<
                                                                    Class<E>::template isSimilar<Float>(), typename Class<DoubleArray>::IfElse<
                                                                            Class<E>::template isSimilar<Double>(), typename Class<StringArray>::IfElse<
                                                                                    Class<E>::template isSimilar<String>(), E>>>>>>>>>;


            /**
             * Return number of element in this array
             */
            virtual gint length() const = 0;

            /**
             * Return true if this array has no element
             */
            virtual gbool isEmpty() const {
                return length() == 0;
            }

            /**
             * Return The element of this array at specified index
             *
             * @param index The position of item
             *
             * @throws IndexException If index out of bounds.
             */
            virtual Value &get(gint index) = 0;

            /**
             * Return The element of this array at specified index
             *
             * @param index The position of item
             *
             * @throws IndexException If index out of bounds.
             */
            virtual const Value &get(gint index) const = 0;

            /**
             * Set the element of this array at specified index
             *
             * @param index The position of item
             *
             * @throws IndexException If index out of bounds.
             */
            virtual void set(gint index, const Value &newValue) = 0;

            /**
             * Return item at specified index
             *
             * @param index
             *              The position of item
             *
             * @throws IndexException
             *              If index out of bounds.
             */
            Value &operator[](gint index) {
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
            Value const &operator[](gint index) const {
                return get(index);
            }

        protected:
            class CIterator CORE_FINAL : public Object {
            private:
                PrimitiveArray &array;
                gint cursor;
                gint limit;

            public:
                CORE_EXPLICIT CIterator(PrimitiveArray &array, gint cursor) :
                        array(array), cursor(cursor), limit(array.length()) {}

                gbool hasNext() const {
                    return cursor < limit && cursor < array.length();
                }

                Value &next() {
                    if (!hasNext()) {
                        NoSuchElementException().throws(__trace("core.native.Primitive.CIterator"));
                    }
                    cursor += 1;
                    return array[cursor - 1];
                }

                gbool equals(const Object &o) const override {
                    if (this == &o) {
                        return true;
                    }
                    if (!Class<CIterator>::hasInstance(o)) {
                        return false;
                    }
                    CIterator const &it = (const CIterator &) o;
                    return &array == &it.array && (cursor == it.cursor && hasNext() == it.hasNext());
                }
            };

            template<class T>
            class LinearIterator CORE_FINAL {
            private:
                CIterator base;

            public:
                CORE_EXPLICIT LinearIterator(const CIterator &base) : base(base) {}

                T &operator*() {
                    return base.next();
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
            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            LinearIterator<Value> begin() {
                return LinearIterator < Value > (CIterator(*this, 0));
            }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the beginning of foreach statement.
             */
            LinearIterator<Value const> begin() const {
                return LinearIterator < Value const>(CIterator((PrimitiveArray &) *this, 0));
            }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            LinearIterator<Value> end() {
                return LinearIterator < Value > (CIterator(*this, length()));
            }

            /**
             * Return The native iterator (The C iterator) used
             * to mark the ending of foreach statement.
             */
            LinearIterator<Value const> end() const {
                return LinearIterator < Value const>(CIterator((PrimitiveArray &) *this, length()));
            }
        };

    } // core
} // private

#endif //CORE23_PRIMITIVEARRAY_H
