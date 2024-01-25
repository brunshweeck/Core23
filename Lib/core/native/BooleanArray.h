//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#ifndef CORE23_BOOLEANARRAY_H
#define CORE23_BOOLEANARRAY_H

#include <core/Boolean.h>
#include <core/MemoryError.h>
#include <core/native/PrimitiveArray.h>

namespace core {
    namespace native {

        /**
         * The BooleanArray class wrap the static array of values from native type
         * (generic) gbool in an object.
         * <p>
         * This class provide the instantaneous access from items
         *
         * @author Brunshweeck Tazeussong
         */
        class BooleanArray CORE_FINAL : public PrimitiveArray<Boolean> {
        private:
            CORE_ALIAS(Value, PrimitiveArray<Boolean>::Value);
            // gbool[*]
            CORE_ALIAS(VALUE, typename Class<Value>::Ptr);

            /**
             * The number of values on this array
             */
            gint len;

            /**
             * The container of this array
             */
            VALUE value;

            CORE_FRATERNITY(::core::native::Unsafe);

        public:

            /**
             * Construct new empty array
             */
            BooleanArray();

            /**
             * Construct new array with specified number
             * of element. all elements are initialized with
             * default initializer (0)
             *
             * @param length The number of elements
             */
            CORE_EXPLICIT BooleanArray(gint length);

            /**
             * Construct new array with specified number
             * of element and initialize elements with specified
             * initial value.
             *
             * @param length The number of elements
             * @param initialValue The value used to initialize all elements of during
             *                      array creation.
             */
            CORE_EXPLICIT BooleanArray(gint length, Value initialValue);

            /**
             * Initialize newly created BooleanArray with items of another.
             *
             * @param array
             *          The root that items are used to initialize this root
             */
            BooleanArray(const BooleanArray &array);

            /**
             * Initialize newly created BooleanArray with items of another.
             *
             * @param array
             *          The root that items are used to initialize this root
             */
            BooleanArray(BooleanArray &&array) CORE_NOTHROW;

            /**
             * Set with items of specified root, all items of this root.
             *
             * @param array
             *          The root that items are used to set this root items
             */
            BooleanArray &operator=(const BooleanArray &array);

            /**
             * Exchange with items of specified root, all items of this root.
             *
             * @param array
             *          The root that items are  exchanged with items of this
             */
            BooleanArray &operator=(BooleanArray &&array) CORE_NOTHROW;

            /**
             * Return number of elements on this array
             */
            gint length() const override;

            /**
             * Tell if this array has no elements
             */
            gbool isEmpty() const override;

            /**
             * Return the element of this array at specified index.
             *
             * @param index The position of item
             * @throws IndexException If index out of bounds.
             */
            Value &get(gint index) override;

            /**
             * Return the element of this array at specified index.
             *
             * @param index The position of item
             * @throws IndexException If index out of bounds.
             */
            Value const &get(gint index) const override;

            /**
             * Set the value of element of this array at specified index.
             *
             * @param index The position of item
             * @param newValue The value used to set element at given index
             * @throws IndexException If index out of bounds.
             */
            void set(gint index, const Value &newValue) override;

            /**
             * Return the shadow copy of this array.
             */
            Object &clone() const override;

            /**
             * Destroy this array
             */
            ~BooleanArray() override;

            /**
             * Return true if specified object is array of same element
             * type and have same elements values as this array
             *
             * @param o The object to be compared
             */
            gbool equals(const Object &o) const override;

            /**
             * Create new empty array
             */
            static BooleanArray of();

            /**
             * Create new array and initialize with one
             * value
             *
             * @param v0 The first value.
             */
            static BooleanArray of(Value v0);

            /**
             * Create new array and initialize with two
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             */
            static BooleanArray of(Value v0, Value v1);

            /**
             * Create new array and initialize with three
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             */
            static BooleanArray of(Value v0, Value v1, Value v2);

            /**
             * Create new array and initialize with four
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The third value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3);

            /**
             * Create new array and initialize with five
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The fourth value
             * @param v4 The fifth value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3,
                                   Value v4);

            /**
             * Create new array and initialize with six
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The fourth value
             * @param v4 The fifth value
             * @param v5 The sixth value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3,
                                   Value v4, Value v5);

            /**
             * Create new array and initialize with seven
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The fourth value
             * @param v4 The fifth value
             * @param v5 The sixth value
             * @param v6 The seventh value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3,
                                   Value v4, Value v5, Value v6);

            /**
             * Create new array and initialize with eight
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The fourth value
             * @param v4 The fifth value
             * @param v5 The sixth value
             * @param v6 The seventh value
             * @param v7 The eighth value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3,
                                   Value v4, Value v5, Value v6, Value v7);

            /**
             * Create new array and initialize with nine
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The fourth value
             * @param v4 The fifth value
             * @param v5 The sixth value
             * @param v6 The seventh value
             * @param v7 The eighth value
             * @param v8 The ninth value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3,
                                   Value v4, Value v5, Value v6, Value v7,
                                   Value v8);

            /**
             * Create new array and initialize with teen
             * values
             *
             * @param v0 The first value.
             * @param v1 The second value
             * @param v2 The third value
             * @param v3 The fourth value
             * @param v4 The fifth value
             * @param v5 The sixth value
             * @param v6 The seventh value
             * @param v7 The eighth value
             * @param v8 The ninth value
             * @param v9 The tenth value
             */
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3,
                                   Value v4, Value v5, Value v6, Value v7,
                                   Value v8, Value v9);

            template<class ...Values>
            static BooleanArray of(Value v0, Value v1, Value v2, Value v3, Value v4, Value v5,
                                   Value v6, Value v7, Value v8, Value v9, Values &&...others) {
                CORE_STATIC_ASSERT(Class<BooleanArray>::allIsTrue(Class<Values>::template isConvertible<Value>()...),
                                   "Could not create array with given arguments");
                try {
                    CORE_FAST gint n = sizeof...(Values);
                    BooleanArray array{10 + n};
                    Value tail[n] = {(Value) ((Values &&) others)...};
                    for (int i = 0; i < n; i += 1) {
                        array[10 + i] = tail[i];
                    }
                    array[0] = v0;
                    array[1] = v1;
                    array[2] = v2;
                    array[3] = v3;
                    array[4] = v4;
                    array[5] = v5;
                    array[6] = v6;
                    array[7] = v7;
                    array[8] = v8;
                    array[9] = v9;
                    return (BooleanArray &&) array;
                } catch (const MemoryError &error) {
                    error.throws();
                }
                return {};
            }
        };

    } // native
} // core

#endif //CORE23_BOOLEANARRAY_H
