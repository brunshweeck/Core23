//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "IntArray2D.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {

        using namespace native;
        using namespace util;

        IntArray2D::IntArray2D() : len(0), value(null) {}

        IntArray2D::IntArray2D(gint rows) : IntArray2D() {
            if (rows < 0)
                IllegalArgumentException("Negative number of rows").throws(__trace("core.native.IntArray2D"));
            if (rows > 0) {
                value = (VALUE) Unsafe::allocateMemory((glong) (rows * sizeof(IntArray)));
                new(value) IntArray[rows];
                len = rows;
            }
        }

        IntArray2D::IntArray2D(gint rows, const IntArray &initialValue) : IntArray2D(rows) {
            if (!initialValue.isEmpty()) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) IntArray(initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~IntArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        IntArray2D::IntArray2D(gint rows, gint columns) : IntArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.IntArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) IntArray(columns);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~IntArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        IntArray2D::IntArray2D(gint rows, gint columns, IntArray2D::Value initialValue) : IntArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.IntArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) IntArray(columns, initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~IntArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        IntArray2D::IntArray2D(const IntArray2D &matrix) : IntArray2D(matrix.len) {
            // CORE_ASSERT(len == matrix.len, "core.native.IntArray2D")
            gint const nRow = matrix.len;
            int i;
            try {
                for (i = 0; i < nRow; i += 1) {
                    IntArray const &row = matrix.value[i];
                    gint const nCol = row.length();
                    if (nCol > 0) {
                        new(value + i) IntArray(nCol);
                    }
                }
            } catch (const MemoryError &error) {
                for (int j = 0; j < i; j += 1) {
                    value[i].~IntArray();
                }
                len = 0;
                Unsafe::freeMemory((glong) value);
                value = null;
                error.throws();
            }
        }

        IntArray2D::IntArray2D(IntArray2D &&matrix) CORE_NOTHROW: IntArray2D() {
            Unsafe::swapValues(value, matrix.value);
            Unsafe::swapValues(len, matrix.len);
        }

        IntArray2D &IntArray2D::operator=(const IntArray2D &matrix) {
            if (this != &matrix) {
                try {
                    IntArray2D copy{matrix};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        IntArray2D &IntArray2D::operator=(IntArray2D &&matrix) CORE_NOTHROW {
            if (this != &matrix) {
                Unsafe::swapValues(value, matrix.value);
                Unsafe::swapValues(len, matrix.len);
            }
            return *this;
        }

        gint IntArray2D::length() const {
            return Math::max(len, 0);
        }

        IntArray &IntArray2D::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        IntArray const &IntArray2D::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        void IntArray2D::set(gint index, const IntArray &newRow) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newRow;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        gint IntArray2D::length(gint row) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i].length();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        IntArray2D::Value &IntArray2D::get(gint row, gint col) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        const IntArray2D::Value &IntArray2D::get(gint row, gint col) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        void IntArray2D::set(gint row, gint col, const IntArray2D::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                value[i].set(col, newValue);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray2D"));
            }
        }

        gbool IntArray2D::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (Class<PrimitiveArray<IntArray>>::hasInstance(o)) {
                PrimitiveArray<IntArray> const &matrix = (PrimitiveArray<IntArray> const &) o;
                gint const n = length();
                if (n != matrix.length())
                    return false;
                try {
                    for (int i = 0; i < n; i += 1) {
                        IntArray const &row1 = value[i];
                        IntArray const &row2 = matrix[i];
                        if (row1 != row2)
                            return false;
                    }
                } catch (const Exception &ex) {
                    return false;
                }
                return true;
            }
            return false;
        }

        Object &IntArray2D::clone() const {
            return Unsafe::allocateInstance<IntArray2D>(*this);
        }

        IntArray2D IntArray2D::of() {
            return {};
        }

        IntArray2D IntArray2D::of(IntArray v0) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D IntArray2D::of(IntArray v0, IntArray v1) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D IntArray2D::of(IntArray v0, IntArray v1, IntArray v2) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D
        IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                matrix.value[4] = Unsafe::moveInstance(v4);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D
        IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4,
                           IntArray v5) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                matrix.value[4] = Unsafe::moveInstance(v4);
                matrix.value[5] = Unsafe::moveInstance(v5);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D
        IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4,
                           IntArray v5, IntArray v6) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                matrix.value[4] = Unsafe::moveInstance(v4);
                matrix.value[5] = Unsafe::moveInstance(v5);
                matrix.value[6] = Unsafe::moveInstance(v6);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D
        IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4,
                           IntArray v5, IntArray v6, IntArray v7) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                matrix.value[4] = Unsafe::moveInstance(v4);
                matrix.value[5] = Unsafe::moveInstance(v5);
                matrix.value[6] = Unsafe::moveInstance(v6);
                matrix.value[7] = Unsafe::moveInstance(v7);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D
        IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4,
                           IntArray v5, IntArray v6, IntArray v7, IntArray v8) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                matrix.value[4] = Unsafe::moveInstance(v4);
                matrix.value[5] = Unsafe::moveInstance(v5);
                matrix.value[6] = Unsafe::moveInstance(v6);
                matrix.value[7] = Unsafe::moveInstance(v7);
                matrix.value[8] = Unsafe::moveInstance(v8);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray2D
        IntArray2D::of(IntArray v0, IntArray v1, IntArray v2, IntArray v3, IntArray v4,
                           IntArray v5, IntArray v6, IntArray v7, IntArray v8, IntArray v9) {
            try {
                IntArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                matrix.value[3] = Unsafe::moveInstance(v3);
                matrix.value[4] = Unsafe::moveInstance(v4);
                matrix.value[5] = Unsafe::moveInstance(v5);
                matrix.value[6] = Unsafe::moveInstance(v6);
                matrix.value[7] = Unsafe::moveInstance(v7);
                matrix.value[8] = Unsafe::moveInstance(v8);
                matrix.value[9] = Unsafe::moveInstance(v9);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // native
} // core
