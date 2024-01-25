//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#ifndef CORE23_STRINGARRAY2D_H
#define CORE23_STRINGARRAY2D_H

#include <core/MemoryError.h>
#include <core/native/StringArray.h>
#include <core/native/PrimitiveArray2D.h>

namespace core {
    namespace native {

        /**
         * The StringArray2D class wrap the static two dimensionally array of values from native type
         * (generic) String in an object.
         *
         * <p>
         * This class provide the instantaneous access from items
         *
         * @author
         *      Brunshweeck Tazeussong
         */
        class StringArray2D CORE_FINAL : public PrimitiveArray2D<String> {
        private:
            // String
            CORE_ALIAS(Value, typename PrimitiveArray2D<String>::Value);
            // String[*][*]
            CORE_ALIAS(VALUE, typename Class<StringArray>::Pointer);
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
            StringArray2D();

            /**
             * Construct new  matrix with specified number of rows.
             * all rows are initialized with zeros columns (This constructor
             * is similar to call StringArray2D(rows, 0)
             *
             * @param rows The number of rows
             */
            CORE_EXPLICIT StringArray2D(gint rows);

            /**
             * Construct new matrix with specified number of rows.
             * all rows are initialized with specified array
             *
             * @param rows The number of rows
             * @param initialValue The value used to initialize all rows
             */
            CORE_EXPLICIT StringArray2D(gint rows, const StringArray &initialValue);

            /**
             * Construct new matrix specified number of rows and given number of
             * columns per rows
             * @param rows The number of rows
             * @param columns The number of columns per row
             */
            CORE_EXPLICIT StringArray2D(gint rows, gint columns);

            /**
             * Construct new matrix and initialize element with given initial value
             *
             * @param rows The number of rows
             * @param columns The number of columns per row
             * @param initialValue The used to initialize all values of this matrix
             */
            CORE_EXPLICIT StringArray2D(gint rows, gint columns, const Value &initialValue);

            /**
             * initialize newly created matrix with other matrix
             *
             * @param matrix The matrix used to initialize this matrix
             */
            StringArray2D(const StringArray2D &matrix);

            /**
             * initialize newly created matrix with other matrix (no risk)
             *
             * @param matrix The matrix used to initialize this matrix
             */
            StringArray2D(StringArray2D &&matrix) CORE_NOTHROW;

            /**
             * Set this matrix content with values other matrix
             *
             * @param matrix The matrix used to initialize this matrix
             */
            StringArray2D &operator=(const StringArray2D &matrix);

            /**
             * Set this matrix content with values other matrix (no risk)
             *
             * @param matrix The matrix used to initialize this matrix
             */
            StringArray2D &operator=(StringArray2D &&matrix) CORE_NOTHROW;

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
            StringArray &get(gint index) override;

            /**
             * Return The array representing the nth row
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            StringArray const &get(gint index) const override;

            /**
             * Set The row at specified position
             *
             * @param index The index of desired row
             * @param newRow The array used to set desired row
             * @throws IndexException If specified index out of bounds
             */
            void set(gint index, const StringArray &newRow) override;

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
             *   get(row).set(col, newValue)
             * @endcode
             *
             * @param row The index of desired row
             * @param col The index of desired column
             * @param newValue The array used to set desired row
             * @throws IndexException If specified index out of bounds
             */
            void set(gint row, gint col, const Value &newValue) override;

            /**
             * Tell if specified object is instance of PrimitiveArray2D<String>
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
            static StringArray2D of();

            /**
             * Create new matrix initialized with one rows
             *
             * @param v0 The first row
             */
            static StringArray2D of(StringArray v0);

            /**
             * Create new matrix initialized with two rows
             *
             * @param v0 The first row
             * @param v1 The second row
             */
            static StringArray2D of(StringArray v0, StringArray v1);

            /**
             * Create new matrix initialized with three rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             */
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2);

            /**
             * Create new matrix initialized with four rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             */
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3);

            /**
             * Create new matrix initialized with five rows
             *
             * @param v0 The first row
             * @param v1 The second row
             * @param v2 The third row
             * @param v3 The fourth row
             * @param v4 The fiftieth row
             */
            static StringArray2D
            of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4);

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
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3,
                                    StringArray v4, StringArray v5);

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
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3,
                                    StringArray v4, StringArray v5, StringArray v6);

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
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3,
                                    StringArray v4, StringArray v5, StringArray v6, StringArray v7);

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
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3,
                                    StringArray v4, StringArray v5, StringArray v6, StringArray v7,
                                    StringArray v8);

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
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3,
                                    StringArray v4, StringArray v5, StringArray v6, StringArray v7,
                                    StringArray v8, StringArray v9);

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
            template<class ...StringArrays>
            static StringArray2D of(StringArray v0, StringArray v1, StringArray v2, StringArray v3,
                                    StringArray v4, StringArray v5, StringArray v6, StringArray v7,
                                    StringArray v8, StringArray v9, StringArrays &&...others) {
                CORE_STATIC_ASSERT(Class<StringArray2D>::allIsTrue(Class<StringArray>::isSimilar<StringArrays>()...),
                                   "Could not create matrix with given arguments");
                try {
                    CORE_FAST gint n = sizeof...(StringArrays);
                    StringArray const tail[n] = {(StringArrays &&) others...};
                    StringArray2D matrix{10 + n};
                    for (int i = 0; i < n; i += 1) {
                        matrix[10 + i] = (StringArray &&) tail[i];
                    }
                    matrix[0] = (StringArray &&) v0;
                    matrix[1] = (StringArray &&) v1;
                    matrix[2] = (StringArray &&) v2;
                    matrix[3] = (StringArray &&) v3;
                    matrix[4] = (StringArray &&) v4;
                    matrix[5] = (StringArray &&) v5;
                    matrix[6] = (StringArray &&) v6;
                    matrix[7] = (StringArray &&) v7;
                    matrix[8] = (StringArray &&) v8;
                    matrix[9] = (StringArray &&) v9;
                    return (StringArray2D &&) matrix;
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

#endif //CORE23_STRINGARRAY2D_H
