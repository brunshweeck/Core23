//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "LongArray2D.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {

        using namespace native;
        using namespace util;

        LongArray2D::LongArray2D() : len(0), value(null) {}

        LongArray2D::LongArray2D(gint rows) : LongArray2D() {
            if (rows < 0)
                IllegalArgumentException("Negative number of rows").throws(__trace("core.native.LongArray2D"));
            if (rows > 0) {
                value = (VALUE) Unsafe::allocateMemory((glong) (rows * sizeof(LongArray)));
                new(value) LongArray[rows];
                len = rows;
            }
        }

        LongArray2D::LongArray2D(gint rows, const LongArray &initialValue) : LongArray2D(rows) {
            if (!initialValue.isEmpty()) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) LongArray(initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~LongArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        LongArray2D::LongArray2D(gint rows, gint columns) : LongArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.LongArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) LongArray(columns);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~LongArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        LongArray2D::LongArray2D(gint rows, gint columns, LongArray2D::Value initialValue) : LongArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.LongArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) LongArray(columns, initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~LongArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        LongArray2D::LongArray2D(const LongArray2D &matrix) : LongArray2D(matrix.len) {
            // CORE_ASSERT(len == matrix.len, "core.native.LongArray2D")
            gint const nRow = matrix.len;
            int i;
            try {
                for (i = 0; i < nRow; i += 1) {
                    LongArray const &row = matrix.value[i];
                    gint const nCol = row.length();
                    if (nCol > 0) {
                        new(value + i) LongArray(nCol);
                    }
                }
            } catch (const MemoryError &error) {
                for (int j = 0; j < i; j += 1) {
                    value[i].~LongArray();
                }
                len = 0;
                Unsafe::freeMemory((glong) value);
                value = null;
                error.throws();
            }
        }

        LongArray2D::LongArray2D(LongArray2D &&matrix) CORE_NOTHROW: LongArray2D() {
            Unsafe::swapValues(value, matrix.value);
            Unsafe::swapValues(len, matrix.len);
        }

        LongArray2D &LongArray2D::operator=(const LongArray2D &matrix) {
            if (this != &matrix) {
                try {
                    LongArray2D copy{matrix};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        LongArray2D &LongArray2D::operator=(LongArray2D &&matrix) CORE_NOTHROW {
            if (this != &matrix) {
                Unsafe::swapValues(value, matrix.value);
                Unsafe::swapValues(len, matrix.len);
            }
            return *this;
        }

        gint LongArray2D::length() const {
            return Math::max(len, 0);
        }

        LongArray &LongArray2D::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        LongArray const &LongArray2D::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        void LongArray2D::set(gint index, const LongArray &newRow) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newRow;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        gint LongArray2D::length(gint row) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i].length();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        LongArray2D::Value &LongArray2D::get(gint row, gint col) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        const LongArray2D::Value &LongArray2D::get(gint row, gint col) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        void LongArray2D::set(gint row, gint col, const LongArray2D::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                value[i].set(col, newValue);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray2D"));
            }
        }

        gbool LongArray2D::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (Class<PrimitiveArray<LongArray>>::hasInstance(o)) {
                PrimitiveArray<LongArray> const &matrix = (PrimitiveArray<LongArray> const &) o;
                gint const n = length();
                if (n != matrix.length())
                    return false;
                try {
                    for (int i = 0; i < n; i += 1) {
                        LongArray const &row1 = value[i];
                        LongArray const &row2 = matrix[i];
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

        Object &LongArray2D::clone() const {
            return Unsafe::allocateInstance<LongArray2D>(*this);
        }

        LongArray2D LongArray2D::of() {
            return {};
        }

        LongArray2D LongArray2D::of(LongArray v0) {
            try {
                LongArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray2D LongArray2D::of(LongArray v0, LongArray v1) {
            try {
                LongArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray2D LongArray2D::of(LongArray v0, LongArray v1, LongArray v2) {
            try {
                LongArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray2D LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3) {
            try {
                LongArray2D matrix{1};
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

        LongArray2D
        LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3, LongArray v4) {
            try {
                LongArray2D matrix{1};
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

        LongArray2D
        LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3, LongArray v4,
                           LongArray v5) {
            try {
                LongArray2D matrix{1};
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

        LongArray2D
        LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3, LongArray v4,
                           LongArray v5, LongArray v6) {
            try {
                LongArray2D matrix{1};
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

        LongArray2D
        LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3, LongArray v4,
                           LongArray v5, LongArray v6, LongArray v7) {
            try {
                LongArray2D matrix{1};
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

        LongArray2D
        LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3, LongArray v4,
                           LongArray v5, LongArray v6, LongArray v7, LongArray v8) {
            try {
                LongArray2D matrix{1};
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

        LongArray2D
        LongArray2D::of(LongArray v0, LongArray v1, LongArray v2, LongArray v3, LongArray v4,
                           LongArray v5, LongArray v6, LongArray v7, LongArray v8, LongArray v9) {
            try {
                LongArray2D matrix{1};
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
