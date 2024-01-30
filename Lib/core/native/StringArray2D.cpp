//
// Created by T.N.Brunshweeck on 19/01/2024.
//

#include "StringArray2D.h"
#include <core/private/Unsafe.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {

        using namespace native;
        using namespace util;

        StringArray2D::StringArray2D() : len(0), value(null) {}

        StringArray2D::StringArray2D(gint rows) : StringArray2D() {
            if (rows < 0)
                IllegalArgumentException("Negative number of rows").throws(__trace("core.native.StringArray2D"));
            if (rows > 0) {
                value = (VALUE) Unsafe::allocateMemory((glong) (rows * sizeof(StringArray)));
                new(value) StringArray[rows];
                len = rows;
            }
        }

        StringArray2D::StringArray2D(gint rows, const StringArray &initialValue) : StringArray2D(rows) {
            if (!initialValue.isEmpty()) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) StringArray(initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~StringArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        StringArray2D::StringArray2D(gint rows, gint columns) : StringArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.StringArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) StringArray(columns);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~StringArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        StringArray2D::StringArray2D(gint rows, gint columns, const StringArray2D::Value& initialValue) : StringArray2D(rows) {
            if (columns < 0)
                IllegalArgumentException("Negative number of columns").throws(__trace("core.native.StringArray2D"));
            if (columns > 0) {
                int i;
                try {
                    for (i = 0; i < rows; i += 1) {
                        new(value + i) StringArray(columns, initialValue);
                    }
                } catch (const MemoryError &error) {
                    // memory is not sufficient to initialize this matrix
                    for (int j = 0; j < i; j += 1) {
                        value[i].~StringArray();
                    }
                    len = 0;
                    Unsafe::freeMemory((glong) value);
                    value = null;
                    error.throws();
                }
            }
        }

        StringArray2D::StringArray2D(const StringArray2D &matrix) : StringArray2D(matrix.len) {
            // CORE_ASSERT(len == matrix.len, "core.native.StringArray2D")
            gint const nRow = matrix.len;
            int i;
            try {
                for (i = 0; i < nRow; i += 1) {
                    StringArray const &row = matrix.value[i];
                    gint const nCol = row.length();
                    if (nCol > 0) {
                        new(value + i) StringArray(nCol);
                    }
                }
            } catch (const MemoryError &error) {
                for (int j = 0; j < i; j += 1) {
                    value[i].~StringArray();
                }
                len = 0;
                Unsafe::freeMemory((glong) value);
                value = null;
                error.throws();
            }
        }

        StringArray2D::StringArray2D(StringArray2D &&matrix) CORE_NOTHROW: StringArray2D() {
            Unsafe::swapValues(value, matrix.value);
            Unsafe::swapValues(len, matrix.len);
        }

        StringArray2D &StringArray2D::operator=(const StringArray2D &matrix) {
            if (this != &matrix) {
                try {
                    StringArray2D copy{matrix};
                    Unsafe::swapValues(value, copy.value);
                    Unsafe::swapValues(len, copy.len);
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
            return *this;
        }

        StringArray2D &StringArray2D::operator=(StringArray2D &&matrix) CORE_NOTHROW {
            if (this != &matrix) {
                Unsafe::swapValues(value, matrix.value);
                Unsafe::swapValues(len, matrix.len);
            }
            return *this;
        }

        gint StringArray2D::length() const {
            return Math::max(len, 0);
        }

        StringArray &StringArray2D::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        StringArray const &StringArray2D::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        void StringArray2D::set(gint index, const StringArray &newRow) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newRow;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        gint StringArray2D::length(gint row) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i].length();
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        StringArray2D::Value &StringArray2D::get(gint row, gint col) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        const StringArray2D::Value &StringArray2D::get(gint row, gint col) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                return value[i][col];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        void StringArray2D::set(gint row, gint col, const StringArray2D::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(row, n);
                value[i].set(col, newValue);
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray2D"));
            }
        }

        gbool StringArray2D::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (Class<PrimitiveArray<StringArray>>::hasInstance(o)) {
                PrimitiveArray<StringArray> const &matrix = (PrimitiveArray<StringArray> const &) o;
                gint const n = length();
                if (n != matrix.length())
                    return false;
                try {
                    for (int i = 0; i < n; i += 1) {
                        StringArray const &row1 = value[i];
                        StringArray const &row2 = matrix[i];
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

        Object &StringArray2D::clone() const {
            return Unsafe::allocateInstance<StringArray2D>(*this);
        }

        StringArray2D StringArray2D::of() {
            return {};
        }

        StringArray2D StringArray2D::of(StringArray v0) {
            try {
                StringArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray2D StringArray2D::of(StringArray v0, StringArray v1) {
            try {
                StringArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray2D StringArray2D::of(StringArray v0, StringArray v1, StringArray v2) {
            try {
                StringArray2D matrix{1};
                matrix.value[0] = Unsafe::moveInstance(v0);
                matrix.value[1] = Unsafe::moveInstance(v1);
                matrix.value[2] = Unsafe::moveInstance(v2);
                return Unsafe::moveInstance(matrix);
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray2D StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3) {
            try {
                StringArray2D matrix{1};
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

        StringArray2D
        StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4) {
            try {
                StringArray2D matrix{1};
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

        StringArray2D
        StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4,
                           StringArray v5) {
            try {
                StringArray2D matrix{1};
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

        StringArray2D
        StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4,
                           StringArray v5, StringArray v6) {
            try {
                StringArray2D matrix{1};
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

        StringArray2D
        StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4,
                           StringArray v5, StringArray v6, StringArray v7) {
            try {
                StringArray2D matrix{1};
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

        StringArray2D
        StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4,
                           StringArray v5, StringArray v6, StringArray v7, StringArray v8) {
            try {
                StringArray2D matrix{1};
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

        StringArray2D
        StringArray2D::of(StringArray v0, StringArray v1, StringArray v2, StringArray v3, StringArray v4,
                           StringArray v5, StringArray v6, StringArray v7, StringArray v8, StringArray v9) {
            try {
                StringArray2D matrix{1};
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
