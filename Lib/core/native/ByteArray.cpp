//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include "ByteArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        ByteArray::ByteArray() : len(0), value(null) {}

        ByteArray::ByteArray(gint length) : ByteArray(length, (gbyte) 0) {}

        ByteArray::ByteArray(gint length, ByteArray::Value initialValue) : ByteArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.ByteArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(Value)));
                    len = length;
                    if (initialValue != 0) {
                        for (int i = 0; i < length; i += 1) {
                            value[i] = initialValue;
                        }
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        ByteArray::ByteArray(const ByteArray &array) : ByteArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        ByteArray::ByteArray(ByteArray &&array) CORE_NOTHROW: ByteArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        ByteArray &ByteArray::operator=(const ByteArray &array) {
            if (this != &array) {
                ByteArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        ByteArray &ByteArray::operator=(ByteArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint ByteArray::length() const {
            return Math::max(len, 0);
        }

        gbool ByteArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        ByteArray::Value &ByteArray::get(gint index) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray"));
            }
        }

        const ByteArray::Value &ByteArray::get(gint index) const {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray"));
            }
        }

        void ByteArray::set(gint index, const ByteArray::Value &newValue) {
            try {
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ByteArray"));
            }
        }

        Object &ByteArray::clone() const {
            return Unsafe::allocateInstance<ByteArray>(*this);
        }

        ByteArray::~ByteArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool ByteArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Byte>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Byte> const &array = (PrimitiveArray<Byte> const &) o;
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

        ByteArray ByteArray::of() {
            return {};
        }

        ByteArray ByteArray::of(ByteArray::Value v0) {
            try {
                ByteArray array{1};
                array.value[0] = v0;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray ByteArray::of(ByteArray::Value v0, ByteArray::Value v1) {
            try {
                ByteArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2) {
            try {
                ByteArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2,
                                ByteArray::Value v3) {
            try {
                ByteArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray
        ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2, ByteArray::Value v3,
                      ByteArray::Value v4) {
            try {
                ByteArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray
        ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2, ByteArray::Value v3,
                      ByteArray::Value v4, ByteArray::Value v5) {
            try {
                ByteArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray
        ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2, ByteArray::Value v3,
                      ByteArray::Value v4, ByteArray::Value v5, ByteArray::Value v6) {
            try {
                ByteArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray
        ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2, ByteArray::Value v3,
                      ByteArray::Value v4, ByteArray::Value v5, ByteArray::Value v6,
                      ByteArray::Value v7) {
            try {
                ByteArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray
        ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2, ByteArray::Value v3,
                      ByteArray::Value v4, ByteArray::Value v5, ByteArray::Value v6, ByteArray::Value v7,
                      ByteArray::Value v8) {
            try {
                ByteArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ByteArray
        ByteArray::of(ByteArray::Value v0, ByteArray::Value v1, ByteArray::Value v2, ByteArray::Value v3,
                      ByteArray::Value v4, ByteArray::Value v5, ByteArray::Value v6, ByteArray::Value v7,
                      ByteArray::Value v8, ByteArray::Value v9) {
            try {
                ByteArray array{10};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                array.value[9] = v9;
                return (ByteArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
