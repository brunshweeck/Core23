//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "CharArray2D.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>
#include <core/AssertionError.h>

namespace core {
    namespace native {

        using namespace native;
        using namespace util;

        CharArray2D::CharArray2D() : len(0), value(null) {}

        CharArray2D::CharArray2D(gint rows) : CharArray2D() {
            if (rows < 0)
                IllegalArgumentException("Negative number of rows").throws(__trace("core.native.CharArray2D"));
            if (rows > 0) {
                value = (VALUE) Unsafe::allocateMemory((glong) (rows * sizeof(CharArray)));
                new(value) CharArray[rows];
                len = rows;
            }
        }

        CharArray2D::CharArray2D(gint rows, const CharArray &initialValue) : CharArray2D(rows) {
            if (!initialValue.isEmpty()) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) CharArray(initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~CharArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        CharArray2D::CharArray2D(gint rows, gint columns) : CharArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.CharArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) CharArray(columns);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~CharArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        CharArray2D::CharArray2D(gint rows, gint columns, CharArray2D::Value initialValue) : CharArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.CharArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) CharArray(columns, initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~CharArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        CharArray2D::CharArray2D(const CharArray2D &matrix) : CharArray2D(matrix.len) {
            CORE_ASSERT(len == matrix.len, __ctrace())
            gint const nRow = matrix.len;
            int i;
            try {
                for (i = 0; i < nRow; i += 1) {
                    CharArray const &row = matrix.value[i];
                    gint const nCol = row.length();
                    if (nCol > 0) {
                        new(value + i) CharArray(nCol);
                    }
                }
            } catch (const MemoryError &error) {
                for (int j = 0; j < i; j += 1) {
                    value[i].~CharArray();
                }
                len = 0;
                Unsafe::freeMemory((glong) value);
                value = null;
                error.throws();
            }
        }

        CharArray2D::CharArray2D(CharArray2D &&matrix) CORE_NOTHROW: CharArray2D() {
            Unsafe::swapValues(value, matrix.value);
            Unsafe::swapValues(len, matrix.len);
        }

        CharArray2D &CharArray2D::operator=(const CharArray2D &matrix) {
            if (this != &matrix) {
                try {
                    CharArray2D copy{matrix};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        CharArray2D &CharArray2D::operator=(CharArray2D &&matrix) CORE_NOTHROW {
            if (this != &matrix) {
                Unsafe::swapValues(value, matrix.value);
                Unsafe::swapValues(len, matrix.len);
            }
            return *this;
        }

        gint CharArray2D::length() const {
            return Math::max(len, 0);
        }

        CharArray &CharArray2D::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        CharArray const &CharArray2D::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        void CharArray2D::set(gint index, const CharArray &newRow) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newRow;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        gint CharArray2D::length(gint row) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i].length();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        CharArray2D::Value &CharArray2D::get(gint row, gint col) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        const CharArray2D::Value &CharArray2D::get(gint row, gint col) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        void CharArray2D::set(gint row, gint col, const CharArray2D::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                value[i].set(col, newValue);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray2D"));
            }
        }

        gbool CharArray2D::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (Class<PrimitiveArray<CharArray>>::hasInstance(o)) {
                PrimitiveArray<CharArray> const &matrix = (PrimitiveArray<CharArray> const &) o;
                gint const n = length();
                if (n != matrix.length())
                    return false;
                try {
                    for (int i = 0; i < n; i += 1) {
                        CharArray const &row1 = value[i];
                        CharArray const &row2 = matrix[i];
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

        Object &CharArray2D::clone() const {
            return Unsafe::allocateInstance<CharArray2D>(*this);
        }

        CharArray2D CharArray2D::of() {
            return {};
        }

        CharArray2D CharArray2D::of(CharArray v0) {
            try {
                CharArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray2D CharArray2D::of(CharArray v0, CharArray v1) {
            try {
                CharArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray2D CharArray2D::of(CharArray v0, CharArray v1, CharArray v2) {
            try {
                CharArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray2D CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3) {
            try {
                CharArray2D matrix{1};
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

        CharArray2D
        CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3, CharArray v4) {
            try {
                CharArray2D matrix{1};
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

        CharArray2D
        CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3, CharArray v4,
                        CharArray v5) {
            try {
                CharArray2D matrix{1};
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

        CharArray2D
        CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3, CharArray v4,
                        CharArray v5, CharArray v6) {
            try {
                CharArray2D matrix{1};
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

        CharArray2D
        CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3, CharArray v4,
                        CharArray v5, CharArray v6, CharArray v7) {
            try {
                CharArray2D matrix{1};
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

        CharArray2D
        CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3, CharArray v4,
                        CharArray v5, CharArray v6, CharArray v7, CharArray v8) {
            try {
                CharArray2D matrix{1};
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

        CharArray2D
        CharArray2D::of(CharArray v0, CharArray v1, CharArray v2, CharArray v3, CharArray v4,
                        CharArray v5, CharArray v6, CharArray v7, CharArray v8, CharArray v9) {
            try {
                CharArray2D matrix{1};
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
