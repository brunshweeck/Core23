//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#ifndef CORE23_PRIMITIVEARRAY2D_H
#define CORE23_PRIMITIVEARRAY2D_H

#include <core/native/PrimitiveArray.h>

namespace core {
    namespace native {

        /**
         * The PrimitiveArray2D represent The Matrix containing element of primitive type
         */
        template<class E>
        class PrimitiveArray2D : public PrimitiveArray<typename PrimitiveArray<E>::Row> {
        protected:
            CORE_ALIAS(Value, typename Class<E>::Primitive);
            CORE_ALIAS(Row, typename PrimitiveArray<E>::Row);

        public:
            /**
             * Return number of rows on this matrix
             */
            virtual gint length() const override = 0;

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
            virtual gint length(gint row) const = 0;

            /**
             * Return The array representing the nth row
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            virtual Row &get(gint index) override = 0;

            /**
             * Return The array representing the nth row
             *
             * @param index The index of desired row
             * @throws IndexException If specified index out of bounds
             */
            virtual Row const &get(gint index) const override = 0;

            /**
             * Set The row at specified position
             *
             * @param index The index of desired row
             * @param newRow The array used to set desired row
             * @throws IndexException If specified index out of bounds
             */
            virtual void set(gint index, Row const &newRow) override = 0;

            /**
             * Return The value representing the nth coordinates
             *
             * @param row The index of desired row
             * @param col The index of desired column
             * @throws IndexException If specified index out of bounds
             */
            virtual Value &get(gint row, gint col) = 0;

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
            virtual Value const &get(gint row, gint col) const = 0;

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
            virtual void set(gint row, gint col, const Value& newValue) = 0;

            virtual gbool equals(const Object &o) const override = 0;

            virtual Object &clone() const override = 0;
        };

    } // native
} // core

#endif //CORE23_PRIMITIVEARRAY2D_H
