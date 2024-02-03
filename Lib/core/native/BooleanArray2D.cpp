//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "BooleanArray2D.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>
#include <core/AssertionError.h>

namespace core {
    namespace native {

        using namespace native;
        using namespace util;

        BooleanArray2D::BooleanArray2D() : len(0), value(null) {}

        BooleanArray2D::BooleanArray2D(gint rows) : BooleanArray2D() {
            if (rows < 0)
                IllegalArgumentException("Negative number of rows").throws(__trace("core.native.BooleanArray2D"));
            if (rows > 0) {
                value = (VALUE) Unsafe::allocateMemory((glong) (rows * sizeof(BooleanArray)));
                new(value) BooleanArray[rows];
                len = rows;
            }
        }

        BooleanArray2D::BooleanArray2D(gint rows, const BooleanArray &initialValue) : BooleanArray2D(rows) {
            if (!initialValue.isEmpty()) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) BooleanArray(initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~BooleanArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        BooleanArray2D::BooleanArray2D(gint rows, gint columns) : BooleanArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.BooleanArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) BooleanArray(columns);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~BooleanArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        BooleanArray2D::BooleanArray2D(gint rows, gint columns, BooleanArray2D::Value initialValue) : BooleanArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.BooleanArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) BooleanArray(columns, initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~BooleanArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        BooleanArray2D::BooleanArray2D(const BooleanArray2D &matrix) : BooleanArray2D(matrix.len) {
             CORE_ASSERT(len == matrix.len, __ctrace());
            gint const nRow = matrix.len;
            int i;
            try {
                for (i = 0; i < nRow; i += 1) {
                    BooleanArray const &row = matrix.value[i];
                    gint const nCol = row.length();
                    if (nCol > 0) {
                        new(value + i) BooleanArray(nCol);
                    }
                }
            } catch (const MemoryError &error) {
                for (int j = 0; j < i; j += 1) {
                    value[i].~BooleanArray();
                }
                len = 0;
                Unsafe::freeMemory((glong) value);
                value = null;
                error.throws();
            }
        }

        BooleanArray2D::BooleanArray2D(BooleanArray2D &&matrix) CORE_NOTHROW: BooleanArray2D() {
            Unsafe::swapValues(value, matrix.value);
            Unsafe::swapValues(len, matrix.len);
        }

        BooleanArray2D &BooleanArray2D::operator=(const BooleanArray2D &matrix) {
            if (this != &matrix) {
                try {
                    BooleanArray2D copy{matrix};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        BooleanArray2D &BooleanArray2D::operator=(BooleanArray2D &&matrix) CORE_NOTHROW {
            if (this != &matrix) {
                Unsafe::swapValues(value, matrix.value);
                Unsafe::swapValues(len, matrix.len);
            }
            return *this;
        }

        gint BooleanArray2D::length() const {
            return Math::max(len, 0);
        }

        BooleanArray &BooleanArray2D::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        BooleanArray const &BooleanArray2D::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        void BooleanArray2D::set(gint index, const BooleanArray &newRow) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newRow;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        gint BooleanArray2D::length(gint row) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i].length();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        BooleanArray2D::Value &BooleanArray2D::get(gint row, gint col) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        const BooleanArray2D::Value &BooleanArray2D::get(gint row, gint col) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        void BooleanArray2D::set(gint row, gint col, const BooleanArray2D::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                value[i].set(col, newValue);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray2D"));
            }
        }

        gbool BooleanArray2D::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (Class<PrimitiveArray<BooleanArray>>::hasInstance(o)) {
                PrimitiveArray<BooleanArray> const &matrix = (PrimitiveArray<BooleanArray> const &) o;
                gint const n = length();
                if (n != matrix.length())
                    return false;
                try {
                    for (int i = 0; i < n; i += 1) {
                        BooleanArray const &row1 = value[i];
                        BooleanArray const &row2 = matrix[i];
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

        Object &BooleanArray2D::clone() const {
            return Unsafe::allocateInstance<BooleanArray2D>(*this);
        }

        BooleanArray2D BooleanArray2D::of() {
            return {};
        }

        BooleanArray2D BooleanArray2D::of(BooleanArray v0) {
            try {
                BooleanArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray2D BooleanArray2D::of(BooleanArray v0, BooleanArray v1) {
            try {
                BooleanArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray2D BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2) {
            try {
                BooleanArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray2D BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3) {
            try {
                BooleanArray2D matrix{1};
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

        BooleanArray2D
        BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3, BooleanArray v4) {
            try {
                BooleanArray2D matrix{1};
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

        BooleanArray2D
        BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3, BooleanArray v4,
                           BooleanArray v5) {
            try {
                BooleanArray2D matrix{1};
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

        BooleanArray2D
        BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3, BooleanArray v4,
                           BooleanArray v5, BooleanArray v6) {
            try {
                BooleanArray2D matrix{1};
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

        BooleanArray2D
        BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3, BooleanArray v4,
                           BooleanArray v5, BooleanArray v6, BooleanArray v7) {
            try {
                BooleanArray2D matrix{1};
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

        BooleanArray2D
        BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3, BooleanArray v4,
                           BooleanArray v5, BooleanArray v6, BooleanArray v7, BooleanArray v8) {
            try {
                BooleanArray2D matrix{1};
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

        BooleanArray2D
        BooleanArray2D::of(BooleanArray v0, BooleanArray v1, BooleanArray v2, BooleanArray v3, BooleanArray v4,
                           BooleanArray v5, BooleanArray v6, BooleanArray v7, BooleanArray v8, BooleanArray v9) {
            try {
                BooleanArray2D matrix{1};
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
