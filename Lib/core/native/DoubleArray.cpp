//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include "DoubleArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        DoubleArray::DoubleArray() : len(0), value(null) {}

        DoubleArray::DoubleArray(gint length) : DoubleArray(length, 0.0) {}

        DoubleArray::DoubleArray(gint length, DoubleArray::Value initialValue) : DoubleArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.DoubleArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(Value)));
                    len = length;
                    if (initialValue != 0.0) {
                        for (int i = 0; i < length; i += 1) {
                            value[i] = initialValue;
                        }
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        DoubleArray::DoubleArray(const DoubleArray &array) : DoubleArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        DoubleArray::DoubleArray(DoubleArray &&array) CORE_NOTHROW: DoubleArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        DoubleArray &DoubleArray::operator=(const DoubleArray &array) {
            if (this != &array) {
                DoubleArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        DoubleArray &DoubleArray::operator=(DoubleArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint DoubleArray::length() const {
            return Math::max(len, 0);
        }

        gbool DoubleArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        DoubleArray::Value &DoubleArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.DoubleArray"));
            }
        }

        const DoubleArray::Value &DoubleArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.DoubleArray"));
            }
        }

        void DoubleArray::set(gint index, const DoubleArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.DoubleArray"));
            }
        }

        Object &DoubleArray::clone() const {
            return Unsafe::allocateInstance<DoubleArray>(*this);
        }

        DoubleArray::~DoubleArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool DoubleArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Double>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Double> const& array = (PrimitiveArray<Double> const&) o;
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

        DoubleArray DoubleArray::of() {
            return {};
        }

        DoubleArray DoubleArray::of(DoubleArray::Value v0) {
            try{
                DoubleArray array{1};
                array.value[0] = v0;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1) {
            try{
                DoubleArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2) {
            try{
                DoubleArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2,
                                      DoubleArray::Value v3) {
            try{
                DoubleArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray
        DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2, DoubleArray::Value v3,
                         DoubleArray::Value v4) {
            try{
                DoubleArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray
        DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2, DoubleArray::Value v3,
                         DoubleArray::Value v4, DoubleArray::Value v5) {
            try{
                DoubleArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray
        DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2, DoubleArray::Value v3,
                         DoubleArray::Value v4, DoubleArray::Value v5, DoubleArray::Value v6) {
            try{
                DoubleArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray
        DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2, DoubleArray::Value v3,
                         DoubleArray::Value v4, DoubleArray::Value v5, DoubleArray::Value v6,
                         DoubleArray::Value v7) {
            try{
                DoubleArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray
        DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2, DoubleArray::Value v3,
                         DoubleArray::Value v4, DoubleArray::Value v5, DoubleArray::Value v6, DoubleArray::Value v7,
                         DoubleArray::Value v8) {
            try{
                DoubleArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        DoubleArray
        DoubleArray::of(DoubleArray::Value v0, DoubleArray::Value v1, DoubleArray::Value v2, DoubleArray::Value v3,
                         DoubleArray::Value v4, DoubleArray::Value v5, DoubleArray::Value v6, DoubleArray::Value v7,
                         DoubleArray::Value v8, DoubleArray::Value v9) {
            try{
                DoubleArray array{10};
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
                return (DoubleArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
