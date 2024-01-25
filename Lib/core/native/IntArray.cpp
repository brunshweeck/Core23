//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include "IntArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        IntArray::IntArray() : len(0), value(null) {}

        IntArray::IntArray(gint length) : IntArray(length, 0) {}

        IntArray::IntArray(gint length, IntArray::Value initialValue) : IntArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.IntArray"));
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

        IntArray::IntArray(const IntArray &array) : IntArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        IntArray::IntArray(IntArray &&array) CORE_NOTHROW: IntArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        IntArray &IntArray::operator=(const IntArray &array) {
            if (this != &array) {
                IntArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        IntArray &IntArray::operator=(IntArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint IntArray::length() const {
            return Math::max(len, 0);
        }

        gbool IntArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        IntArray::Value &IntArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray"));
            }
        }

        const IntArray::Value &IntArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray"));
            }
        }

        void IntArray::set(gint index, const IntArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.IntArray"));
            }
        }

        Object &IntArray::clone() const {
            return Unsafe::allocateInstance<IntArray>(*this);
        }

        IntArray::~IntArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool IntArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Integer>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Integer> const& array = (PrimitiveArray<Integer> const&) o;
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

        IntArray IntArray::of() {
            return {};
        }

        IntArray IntArray::of(IntArray::Value v0) {
            try{
                IntArray array{1};
                array.value[0] = v0;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray IntArray::of(IntArray::Value v0, IntArray::Value v1) {
            try{
                IntArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2) {
            try{
                IntArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2,
                                      IntArray::Value v3) {
            try{
                IntArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray
        IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2, IntArray::Value v3,
                         IntArray::Value v4) {
            try{
                IntArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray
        IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2, IntArray::Value v3,
                         IntArray::Value v4, IntArray::Value v5) {
            try{
                IntArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray
        IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2, IntArray::Value v3,
                         IntArray::Value v4, IntArray::Value v5, IntArray::Value v6) {
            try{
                IntArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray
        IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2, IntArray::Value v3,
                         IntArray::Value v4, IntArray::Value v5, IntArray::Value v6,
                         IntArray::Value v7) {
            try{
                IntArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray
        IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2, IntArray::Value v3,
                         IntArray::Value v4, IntArray::Value v5, IntArray::Value v6, IntArray::Value v7,
                         IntArray::Value v8) {
            try{
                IntArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        IntArray
        IntArray::of(IntArray::Value v0, IntArray::Value v1, IntArray::Value v2, IntArray::Value v3,
                         IntArray::Value v4, IntArray::Value v5, IntArray::Value v6, IntArray::Value v7,
                         IntArray::Value v8, IntArray::Value v9) {
            try{
                IntArray array{10};
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
                return (IntArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
