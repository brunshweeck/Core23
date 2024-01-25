//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include "BooleanArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        BooleanArray::BooleanArray() : len(0), value(null) {}

        BooleanArray::BooleanArray(gint length) : BooleanArray(length, false) {}

        BooleanArray::BooleanArray(gint length, BooleanArray::Value initialValue) : BooleanArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.BooleanArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(Value)));
                    len = length;
                    if (initialValue) {
                        for (int i = 0; i < length; i += 1) {
                            value[i] = initialValue;
                        }
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        BooleanArray::BooleanArray(const BooleanArray &array) : BooleanArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        BooleanArray::BooleanArray(BooleanArray &&array) CORE_NOTHROW: BooleanArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        BooleanArray &BooleanArray::operator=(const BooleanArray &array) {
            if (this != &array) {
                BooleanArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        BooleanArray &BooleanArray::operator=(BooleanArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint BooleanArray::length() const {
            return Math::max(len, 0);
        }

        gbool BooleanArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        BooleanArray::Value &BooleanArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray"));
            }
        }

        const BooleanArray::Value &BooleanArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray"));
            }
        }

        void BooleanArray::set(gint index, const BooleanArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.BooleanArray"));
            }
        }

        Object &BooleanArray::clone() const {
            return Unsafe::allocateInstance<BooleanArray>(*this);
        }

        BooleanArray::~BooleanArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool BooleanArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Boolean>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Boolean> const& array = (PrimitiveArray<Boolean> const&) o;
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

        BooleanArray BooleanArray::of() {
            return {};
        }

        BooleanArray BooleanArray::of(BooleanArray::Value v0) {
            try{
                BooleanArray array{1};
                array.value[0] = v0;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1) {
            try{
                BooleanArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2) {
            try{
                BooleanArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2,
                                      BooleanArray::Value v3) {
            try{
                BooleanArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray
        BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2, BooleanArray::Value v3,
                         BooleanArray::Value v4) {
            try{
                BooleanArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray
        BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2, BooleanArray::Value v3,
                         BooleanArray::Value v4, BooleanArray::Value v5) {
            try{
                BooleanArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray
        BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2, BooleanArray::Value v3,
                         BooleanArray::Value v4, BooleanArray::Value v5, BooleanArray::Value v6) {
            try{
                BooleanArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray
        BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2, BooleanArray::Value v3,
                         BooleanArray::Value v4, BooleanArray::Value v5, BooleanArray::Value v6,
                         BooleanArray::Value v7) {
            try{
                BooleanArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray
        BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2, BooleanArray::Value v3,
                         BooleanArray::Value v4, BooleanArray::Value v5, BooleanArray::Value v6, BooleanArray::Value v7,
                         BooleanArray::Value v8) {
            try{
                BooleanArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        BooleanArray
        BooleanArray::of(BooleanArray::Value v0, BooleanArray::Value v1, BooleanArray::Value v2, BooleanArray::Value v3,
                         BooleanArray::Value v4, BooleanArray::Value v5, BooleanArray::Value v6, BooleanArray::Value v7,
                         BooleanArray::Value v8, BooleanArray::Value v9) {
            try{
                BooleanArray array{10};
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
                return (BooleanArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
