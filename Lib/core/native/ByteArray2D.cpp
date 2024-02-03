//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "ByteArray2D.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>
#include <core/AssertionError.h>

namespace core {
    namespace native {

        using namespace native;
        using namespace util;

        ByteArray2D::ByteArray2D() : len(0), value(null) {}

        ByteArray2D::ByteArray2D(gint rows) : ByteArray2D() {
            if (rows < 0)
                IllegalArgumentException("Negative number of rows").throws(__trace("core.native.ByteArray2D"));
            if (rows > 0) {
                value = (VALUE) Unsafe::allocateMemory((glong) (rows * sizeof(ByteArray)));
                new(value) ByteArray[rows];
                len = rows;
            }
        }

        ByteArray2D::ByteArray2D(gint rows, const ByteArray &initialValue) : ByteArray2D(rows) {
            if (!initialValue.isEmpty()) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) ByteArray(initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~ByteArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        ByteArray2D::ByteArray2D(gint rows, gint columns) : ByteArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.ByteArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) ByteArray(columns);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~ByteArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        ByteArray2D::ByteArray2D(gint rows, gint columns, ByteArray2D::Value initialValue) : ByteArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.ByteArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) ByteArray(columns, initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~ByteArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        ByteArray2D::ByteArray2D(const ByteArray2D &matrix) : ByteArray2D(matrix.len) {
             CORE_ASSERT(len == matrix.len, __ctrace())
            gint const nRow = matrix.len;
            int i;
            try {
                for (i = 0; i < nRow; i += 1) {
                    ByteArray const &row = matrix.value[i];
                    gint const nCol = row.length();
                    if (nCol > 0) {
                        new(value + i) ByteArray(nCol);
                    }
                }
            } catch (const MemoryError &error) {
                for (int j = 0; j < i; j += 1) {
                    value[i].~ByteArray();
                }
                len = 0;
                Unsafe::freeMemory((glong) value);
                value = null;
                error.throws();
            }
        }

        ByteArray2D::ByteArray2D(ByteArray2D &&matrix) CORE_NOTHROW: ByteArray2D() {
            Unsafe::swapValues(value, matrix.value);
            Unsafe::swapValues(len, matrix.len);
        }

        ByteArray2D &ByteArray2D::operator=(const ByteArray2D &matrix) {
            if (this != &matrix) {
                try {
                    ByteArray2D copy{matrix};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        ByteArray2D &ByteArray2D::operator=(ByteArray2D &&matrix) CORE_NOTHROW {
            if (this != &matrix) {
                Unsafe::swapValues(value, matrix.value);
                Unsafe::swapValues(len, matrix.len);
            }
            return *this;
        }

        gint ByteArray2D::length() const {
            return Math::max(len, 0);
        }

        ByteArray &ByteArray2D::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        ByteArray const &ByteArray2D::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        void ByteArray2D::set(gint index, const ByteArray &newRow) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newRow;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        gint ByteArray2D::length(gint row) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i].length();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        ByteArray2D::Value &ByteArray2D::get(gint row, gint col) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        const ByteArray2D::Value &ByteArray2D::get(gint row, gint col) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        void ByteArray2D::set(gint row, gint col, const ByteArray2D::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                value[i].set(col, newValue);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray2D"));
            }
        }

        gbool ByteArray2D::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (Class<PrimitiveArray<ByteArray>>::hasInstance(o)) {
                PrimitiveArray<ByteArray> const &matrix = (PrimitiveArray<ByteArray> const &) o;
                gint const n = length();
                if (n != matrix.length())
                    return false;
                try {
                    for (int i = 0; i < n; i += 1) {
                        ByteArray const &row1 = value[i];
                        ByteArray const &row2 = matrix[i];
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

        Object &ByteArray2D::clone() const {
            return Unsafe::allocateInstance<ByteArray2D>(*this);
        }

        ByteArray2D ByteArray2D::of() {
            return {};
        }

        ByteArray2D ByteArray2D::of(ByteArray v0) {
            try {
                ByteArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray2D ByteArray2D::of(ByteArray v0, ByteArray v1) {
            try {
                ByteArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray2D ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2) {
            try {
                ByteArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray2D ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3) {
            try {
                ByteArray2D matrix{1};
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

        ByteArray2D
        ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3, ByteArray v4) {
            try {
                ByteArray2D matrix{1};
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

        ByteArray2D
        ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3, ByteArray v4,
                           ByteArray v5) {
            try {
                ByteArray2D matrix{1};
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

        ByteArray2D
        ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3, ByteArray v4,
                           ByteArray v5, ByteArray v6) {
            try {
                ByteArray2D matrix{1};
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

        ByteArray2D
        ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3, ByteArray v4,
                           ByteArray v5, ByteArray v6, ByteArray v7) {
            try {
                ByteArray2D matrix{1};
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

        ByteArray2D
        ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3, ByteArray v4,
                           ByteArray v5, ByteArray v6, ByteArray v7, ByteArray v8) {
            try {
                ByteArray2D matrix{1};
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

        ByteArray2D
        ByteArray2D::of(ByteArray v0, ByteArray v1, ByteArray v2, ByteArray v3, ByteArray v4,
                           ByteArray v5, ByteArray v6, ByteArray v7, ByteArray v8, ByteArray v9) {
            try {
                ByteArray2D matrix{1};
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
