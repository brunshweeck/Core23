//
// Created by T.N.Brunshweeck on 14/12/2023.
//



#include "StringArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        StringArray::StringArray() : len(0), value(null) {}

        StringArray::StringArray(gint length) : StringArray(length, {}) {}

        StringArray::StringArray(gint length, const StringArray::Value &initialValue) : StringArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.StringArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(Value)));
                    if (initialValue.isEmpty()) {
                        new(value) String[length];
                        len = length;
                    } else {
                        try {
                            for (int i = 0; i < length; i += 1) {
                                new(value + i) String(initialValue);
                            }
                            len = length;
                        } catch (const MemoryError &error) {
                            Unsafe::freeMemory((glong) value);
                            value = null;
                            error.throws();
                        }
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        StringArray::StringArray(const StringArray &array) : StringArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        StringArray::StringArray(StringArray &&array) CORE_NOTHROW: StringArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        StringArray &StringArray::operator=(const StringArray &array) {
            if (this != &array) {
                StringArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        StringArray &StringArray::operator=(StringArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint StringArray::length() const {
            return Math::max(len, 0);
        }

        gbool StringArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        StringArray::Value &StringArray::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray"));
            }
        }

        const StringArray::Value &StringArray::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray"));
            }
        }

        void StringArray::set(gint index, const StringArray::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.StringArray"));
            }
        }

        Object &StringArray::clone() const {
            return Unsafe::allocateInstance<StringArray>(*this);
        }

        StringArray::~StringArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool StringArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<String>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<String> const &array = (PrimitiveArray<String> const &) o;
            gint const n = length();
            if (n != array.length()) {
                return false;
            }
            try {
                for (int i = 0; i < n; i += 1) {
                    if (value[i] != array[i]) {
                        return false;
                    }
                }
            } catch (const Throwable &th) {
                return false;
            }
            return true;
        }

        StringArray StringArray::of() {
            return {};
        }

        StringArray StringArray::of(StringArray::Value v0) {
            try {
                StringArray array{1};
                array.value[0] = (String &&) v0;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray StringArray::of(StringArray::Value v0, StringArray::Value v1) {
            try {
                StringArray array{2};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2) {
            try {
                StringArray array{3};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2,
                                    StringArray::Value v3) {
            try {
                StringArray array{4};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray
        StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2, StringArray::Value v3,
                        StringArray::Value v4) {
            try {
                StringArray array{5};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                array.value[4] = (String &&) v4;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray
        StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2, StringArray::Value v3,
                        StringArray::Value v4, StringArray::Value v5) {
            try {
                StringArray array{6};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                array.value[4] = (String &&) v4;
                array.value[5] = (String &&) v5;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray
        StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2, StringArray::Value v3,
                        StringArray::Value v4, StringArray::Value v5, StringArray::Value v6) {
            try {
                StringArray array{7};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                array.value[4] = (String &&) v4;
                array.value[5] = (String &&) v5;
                array.value[6] = (String &&) v6;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray
        StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2, StringArray::Value v3,
                        StringArray::Value v4, StringArray::Value v5, StringArray::Value v6,
                        StringArray::Value v7) {
            try {
                StringArray array{8};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                array.value[4] = (String &&) v4;
                array.value[5] = (String &&) v5;
                array.value[6] = (String &&) v6;
                array.value[7] = (String &&) v7;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray
        StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2, StringArray::Value v3,
                        StringArray::Value v4, StringArray::Value v5, StringArray::Value v6, StringArray::Value v7,
                        StringArray::Value v8) {
            try {
                StringArray array{9};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                array.value[4] = (String &&) v4;
                array.value[5] = (String &&) v5;
                array.value[6] = (String &&) v6;
                array.value[7] = (String &&) v7;
                array.value[8] = (String &&) v8;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        StringArray
        StringArray::of(StringArray::Value v0, StringArray::Value v1, StringArray::Value v2, StringArray::Value v3,
                        StringArray::Value v4, StringArray::Value v5, StringArray::Value v6, StringArray::Value v7,
                        StringArray::Value v8, StringArray::Value v9) {
            try {
                StringArray array{10};
                array.value[0] = (String &&) v0;
                array.value[1] = (String &&) v1;
                array.value[2] = (String &&) v2;
                array.value[3] = (String &&) v3;
                array.value[4] = (String &&) v4;
                array.value[5] = (String &&) v5;
                array.value[6] = (String &&) v6;
                array.value[7] = (String &&) v7;
                array.value[8] = (String &&) v8;
                array.value[9] = (String &&) v9;
                return (StringArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
