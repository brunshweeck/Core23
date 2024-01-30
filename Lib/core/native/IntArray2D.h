//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#ifndef CORE23_INTARRAY2D_H
#define CORE23_INTARRAY2D_H

#include <core/MemoryError.h>
#include <core/native/IntArray.h>
#include <core/native/PrimitiveArray2D.h>

namespace core {
    namespace native {

        /**
         * The IntArray2D class wrap the static two dimensionally array of values from native type
         * (generic) gint in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class IntArray2D CORE_FINAL : public PrimitiveArray2D<Integer> {
        private:
            // gint
            CORE_ALIAS(Value, typename PrimitiveArray2D<Integer>::Value);
            // gint[*][*]
            CORE_ALIAS(VALUE, typename Class<IntArray>::Pointer);
            /**
             * The number of rows on this matrix
             */
            gint len;

            /**
             * The container of this matrix
             */
            VALUE value;

        public:

            /**
             * Construct new empty matrix
             */
            IntArray2D();

            /**
             * Construct new  matrix with specified number of rows.
             * all rows are initialized with zeros columns (This constructor
             * is similar to call IntArray2D(rows, 0)
             *
             * @param rows The number of rows
             */
            CORE_EXPLICIT IntArray2D(gint rows);

            /**
             * Construct new matrix with specified number of rows.
             * all rows are initialized with specified array
             *
             * @param rows The number of rows
             * @param initialValue The value used to initialize all rows
             */
            CORE_EXPLICIT IntArray2D(gint rows, const IntArray &initialValue);

            /**
             * Construct new matrix specified number of rows and given number of
             * columns per rows
             * @param rows The number of rows
             * @param columns The number of columns per row
             */
            CORE_EXPLICIT IntArray2D(gint rows, gint columns);

            /**
             * Construct new matrix and initialize element with given initial value
             *
             * @param rows The number of rows
             * @param columns The number of columns per row
             * @param initialValue The used to initialize all values of this matrix
             */
            CORE_EXPLICIT IntArray2D(gint rows, gint columns, Value initialValue);

            /**
             * initialize newly created matrix with other matrix
             *
             * @param matrix The matrix used to initialize this matrix
             */
            IntArray2D(const IntArray2D &matrix);

            /**
             * initialize newly created matrix with other matrix (no risk)
             *
             * @param matrix The matrix used to initialize this matrix
             */
            IntArray2D(IntArray2D &&matrix) CORE_NOTHROW;

            /**
             * Set this matrix content with values other matrix
             *
             * @param matrix The matrix used to initialize this matrix
             */
            IntArray2D &operator=(const IntArray2D &matrix);

            /**
             * Set this matrix content with values other matrix (no risk)
             *
             * @param matrix The matrix used to initialize this matrix
             */
            IntArray2D &operator=(IntArray2D &&matrix) CORE_NOTHROW;

            /**
             * Return number of rows of this matrix
             */
            gint length() const override;

            /**
             * Return The array representing the nth row
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            IntArray &get(gint index) override;

            /**
             * Return The array representing the nth row
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            IntArray const &get(gint index) const override;

            /**
             * Set The row at specified position
             *
             * @param index The index of desired row
             * @param newRow The array used to setValue desired row
             * @throws IndexException If specified index out of bounds
             */
            void set(gint index, const IntArray &newRow) override;

            /**
             * Return number of columns of row at specified index on this matrix.
             * It equivalent to call
             * @code
             *   get(row).length()
             * @endcode
             *
             * @param row The index of desired row
             * @throws IndexException if specified index out of bounds.
             */
            gint length(gint row) const override;

            /**
             * Return The array representing the nth row.
             * This call is equivalent at:
             * @code
             *   get(row).get(col)
             * @endcode
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            Value &get(gint row, gint col) override;

            /**
             * Return The array representing the nth row.
             * This call is equivalent at:
             * @code
             *   get(row).get(col)
             * @endcode
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            const Value &get(gint row, gint col) const override;

            /**
             * Set The value at specified position.
             * This call is equivalent at:
             * @code
             *   get(row).setValue(col, newValue)
             * @endcode
             *
             * @param row The index of desired row
             * @param col The index of desired column
             * @param newValue The array used to setValue desired row
             * @throws IndexException If specified index out of bounds
             */
            void set(gint row, gint col, const Value &newValue) override;

            /**
             * Tell if specified object is instance of PrimitiveArray2D<Integer>
             * with same element types and same values
             */
            gbool equals(const Object &o) const override;

            /**
             * Return shadow copy of this matrix.
             */
            Object &clone() const override;

            /**
             * Create new empty matrix
             *
             */
            static IntArray2D of();

            /**
             * Create new matrix initialized with one rows
             *
             * @param v0 The first row
             */
            static IntArray2D of(IntArray v0);

            /**
             * Create new matrix initialized with two rows
             *
             * @param v0 The first row
             * @param v1 The second row
             */
            static IntArray2D of(IntArray v0, IntArray v1);

            /**
             * Create new matrix initialized with three rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2);

            /**
             * Create new matrix initialized with four rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3);

            /**
             * Create new matrix initialized with five rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             */
            static IntArray2D
            of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4);

            /**
             * Create new matrix initialized with six rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             * @param v5 The sixth row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3,
                                   IntArray v4, IntArray v5);

            /**
             * Create new matrix initialized with seven rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             * @param v5 The sixth row
             * @param v6 The seventh row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3,
                                   IntArray v4, IntArray v5, IntArray v6);

            /**
             * Create new matrix initialized with eight rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             * @param v5 The sixth row
             * @param v6 The seventh row
             * @param v7 The eighth row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3,
                                   IntArray v4, IntArray v5, IntArray v6, IntArray v7);

            /**
             * Create new matrix initialized with nine rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             * @param v5 The sixth row
             * @param v6 The seventh row
             * @param v7 The eighth row
             * @param v8 The ninth row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3,
                                   IntArray v4, IntArray v5, IntArray v6, IntArray v7,
                                   IntArray v8);

            /**
             * Create new matrix initialized with ten rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             * @param v5 The sixth row
             * @param v6 The seventh row
             * @param v7 The eighth row
             * @param v8 The ninth row
             * @param v9 The tenth row
             */
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3,
                                   IntArray v4, IntArray v5, IntArray v6, IntArray v7,
                                   IntArray v8, IntArray v9);

            /**
             * Create new matrix initialized with ten and more rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             * @param v5 The sixth row
             * @param v6 The seventh row
             * @param v7 The eighth row
             * @param v8 The ninth row
             * @param v9 The tenth row
             * @param others The more rows
             */
            template<class ...IntArrays>
            static IntArray2D of(IntArray v0, IntArray v1, IntArray v2, IntArray v3,
                                   IntArray v4, IntArray v5, IntArray v6, IntArray v7,
                                   IntArray v8, IntArray v9, IntArrays &&...others) {
                CORE_STATIC_ASSERT(Class<IntArray2D>::template allIsTrue<Class<IntArray>::isSimilar<IntArrays>()...>(),
                                   "Could not create matrix with given arguments");
                try{
                    CORE_FAST gint n = sizeof...(IntArrays);
                    IntArray const tail[n] = {(IntArrays &&) others...};
                    IntArray2D matrix{10 + n};
                    for (int i = 0; i < n; i += 1) {
                        matrix[10 + i] = (IntArray &&) tail[i];
                    }
                    matrix[0] = (IntArray &&) v0;
                    matrix[1] = (IntArray &&) v1;
                    matrix[2] = (IntArray &&) v2;
                    matrix[3] = (IntArray &&) v3;
                    matrix[4] = (IntArray &&) v4;
                    matrix[5] = (IntArray &&) v5;
                    matrix[6] = (IntArray &&) v6;
                    matrix[7] = (IntArray &&) v7;
                    matrix[8] = (IntArray &&) v8;
                    matrix[9] = (IntArray &&) v9;
                    return (IntArray2D &&) matrix;
                } catch (const MemoryError &error) {
                    // (matrix allocation fail) ignore this point and rethrow
                    error.throws();
                }
                // unreachable
                return {};
            }
        };

    } // native
} // core

#endif //CORE23_INTARRAY2D_H
