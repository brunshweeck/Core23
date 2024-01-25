//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include "ShortArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        ShortArray::ShortArray() : len(0), value(null) {}

        ShortArray::ShortArray(gint length) : ShortArray(length, (gshort)0) {}

        ShortArray::ShortArray(gint length, ShortArray::Value initialValue) : ShortArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.ShortArray"));
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

        ShortArray::ShortArray(const ShortArray &array) : ShortArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        ShortArray::ShortArray(ShortArray &&array) CORE_NOTHROW: ShortArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        ShortArray &ShortArray::operator=(const ShortArray &array) {
            if (this != &array) {
                ShortArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        ShortArray &ShortArray::operator=(ShortArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint ShortArray::length() const {
            return Math::max(len, 0);
        }

        gbool ShortArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        ShortArray::Value &ShortArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ShortArray"));
            }
        }

        const ShortArray::Value &ShortArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ShortArray"));
            }
        }

        void ShortArray::set(gint index, const ShortArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.ShortArray"));
            }
        }

        Object &ShortArray::clone() const {
            return Unsafe::allocateInstance<ShortArray>(*this);
        }

        ShortArray::~ShortArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool ShortArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Short>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Short> const& array = (PrimitiveArray<Short> const&) o;
            gint const n = length();
            if(n != array.length()){
                return false;
            }
            try{
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

        ShortArray ShortArray::of() {
            return {};
        }

        ShortArray ShortArray::of(ShortArray::Value v0) {
            try{
                ShortArray array{1};
                array.value[0] = v0;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray ShortArray::of(ShortArray::Value v0, ShortArray::Value v1) {
            try{
                ShortArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2) {
            try{
                ShortArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2,
                                      ShortArray::Value v3) {
            try{
                ShortArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray
        ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2, ShortArray::Value v3,
                         ShortArray::Value v4) {
            try{
                ShortArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray
        ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2, ShortArray::Value v3,
                         ShortArray::Value v4, ShortArray::Value v5) {
            try{
                ShortArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray
        ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2, ShortArray::Value v3,
                         ShortArray::Value v4, ShortArray::Value v5, ShortArray::Value v6) {
            try{
                ShortArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray
        ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2, ShortArray::Value v3,
                         ShortArray::Value v4, ShortArray::Value v5, ShortArray::Value v6,
                         ShortArray::Value v7) {
            try{
                ShortArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray
        ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2, ShortArray::Value v3,
                         ShortArray::Value v4, ShortArray::Value v5, ShortArray::Value v6, ShortArray::Value v7,
                         ShortArray::Value v8) {
            try{
                ShortArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        ShortArray
        ShortArray::of(ShortArray::Value v0, ShortArray::Value v1, ShortArray::Value v2, ShortArray::Value v3,
                         ShortArray::Value v4, ShortArray::Value v5, ShortArray::Value v6, ShortArray::Value v7,
                         ShortArray::Value v8, ShortArray::Value v9) {
            try{
                ShortArray array{10};
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
                return (ShortArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
