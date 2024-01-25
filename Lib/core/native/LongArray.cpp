//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include "LongArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        LongArray::LongArray() : len(0), value(null) {}

        LongArray::LongArray(gint length) : LongArray(length, 0L) {}

        LongArray::LongArray(gint length, LongArray::Value initialValue) : LongArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.LongArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(Value)));
                    len = length;
                    if (initialValue != 0L) {
                        for (int i = 0; i < length; i += 1) {
                            value[i] = initialValue;
                        }
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        LongArray::LongArray(const LongArray &array) : LongArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        LongArray::LongArray(LongArray &&array) CORE_NOTHROW: LongArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        LongArray &LongArray::operator=(const LongArray &array) {
            if (this != &array) {
                LongArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        LongArray &LongArray::operator=(LongArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint LongArray::length() const {
            return Math::max(len, 0);
        }

        gbool LongArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        LongArray::Value &LongArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray"));
            }
        }

        const LongArray::Value &LongArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray"));
            }
        }

        void LongArray::set(gint index, const LongArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.LongArray"));
            }
        }

        Object &LongArray::clone() const {
            return Unsafe::allocateInstance<LongArray>(*this);
        }

        LongArray::~LongArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool LongArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Long>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Long> const& array = (PrimitiveArray<Long> const&) o;
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

        LongArray LongArray::of() {
            return {};
        }

        LongArray LongArray::of(LongArray::Value v0) {
            try{
                LongArray array{1};
                array.value[0] = v0;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray LongArray::of(LongArray::Value v0, LongArray::Value v1) {
            try{
                LongArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2) {
            try{
                LongArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2,
                                      LongArray::Value v3) {
            try{
                LongArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray
        LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2, LongArray::Value v3,
                         LongArray::Value v4) {
            try{
                LongArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray
        LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2, LongArray::Value v3,
                         LongArray::Value v4, LongArray::Value v5) {
            try{
                LongArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray
        LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2, LongArray::Value v3,
                         LongArray::Value v4, LongArray::Value v5, LongArray::Value v6) {
            try{
                LongArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray
        LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2, LongArray::Value v3,
                         LongArray::Value v4, LongArray::Value v5, LongArray::Value v6,
                         LongArray::Value v7) {
            try{
                LongArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray
        LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2, LongArray::Value v3,
                         LongArray::Value v4, LongArray::Value v5, LongArray::Value v6, LongArray::Value v7,
                         LongArray::Value v8) {
            try{
                LongArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        LongArray
        LongArray::of(LongArray::Value v0, LongArray::Value v1, LongArray::Value v2, LongArray::Value v3,
                         LongArray::Value v4, LongArray::Value v5, LongArray::Value v6, LongArray::Value v7,
                         LongArray::Value v8, LongArray::Value v9) {
            try{
                LongArray array{10};
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
                return (LongArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
