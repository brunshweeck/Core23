//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include "FloatArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        FloatArray::FloatArray() : len(0), value(null) {}

        FloatArray::FloatArray(gint length) : FloatArray(length, 0.0F) {}

        FloatArray::FloatArray(gint length, FloatArray::Value initialValue) : FloatArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.FloatArray"));
            if (length > 0) {
                try {
                    value = (VALUE) Unsafe::allocateMemory((glong) (length * sizeof(Value)));
                    len = length;
                    if (initialValue != 0.0F) {
                        for (int i = 0; i < length; i += 1) {
                            value[i] = initialValue;
                        }
                    }
                } catch (const MemoryError &error) {
                    error.throws();
                }
            }
        }

        FloatArray::FloatArray(const FloatArray &array) : FloatArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        FloatArray::FloatArray(FloatArray &&array) CORE_NOTHROW: FloatArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        FloatArray &FloatArray::operator=(const FloatArray &array) {
            if (this != &array) {
                FloatArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        FloatArray &FloatArray::operator=(FloatArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint FloatArray::length() const {
            return Math::max(len, 0);
        }

        gbool FloatArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        FloatArray::Value &FloatArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.FloatArray"));
            }
        }

        const FloatArray::Value &FloatArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.FloatArray"));
            }
        }

        void FloatArray::set(gint index, const FloatArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.FloatArray"));
            }
        }

        Object &FloatArray::clone() const {
            return Unsafe::allocateInstance<FloatArray>(*this);
        }

        FloatArray::~FloatArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool FloatArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Float>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Float> const& array = (PrimitiveArray<Float> const&) o;
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

        FloatArray FloatArray::of() {
            return {};
        }

        FloatArray FloatArray::of(FloatArray::Value v0) {
            try{
                FloatArray array{1};
                array.value[0] = v0;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray FloatArray::of(FloatArray::Value v0, FloatArray::Value v1) {
            try{
                FloatArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2) {
            try{
                FloatArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2,
                                      FloatArray::Value v3) {
            try{
                FloatArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray
        FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2, FloatArray::Value v3,
                         FloatArray::Value v4) {
            try{
                FloatArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray
        FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2, FloatArray::Value v3,
                         FloatArray::Value v4, FloatArray::Value v5) {
            try{
                FloatArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray
        FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2, FloatArray::Value v3,
                         FloatArray::Value v4, FloatArray::Value v5, FloatArray::Value v6) {
            try{
                FloatArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray
        FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2, FloatArray::Value v3,
                         FloatArray::Value v4, FloatArray::Value v5, FloatArray::Value v6,
                         FloatArray::Value v7) {
            try{
                FloatArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray
        FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2, FloatArray::Value v3,
                         FloatArray::Value v4, FloatArray::Value v5, FloatArray::Value v6, FloatArray::Value v7,
                         FloatArray::Value v8) {
            try{
                FloatArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        FloatArray
        FloatArray::of(FloatArray::Value v0, FloatArray::Value v1, FloatArray::Value v2, FloatArray::Value v3,
                         FloatArray::Value v4, FloatArray::Value v5, FloatArray::Value v6, FloatArray::Value v7,
                         FloatArray::Value v8, FloatArray::Value v9) {
            try{
                FloatArray array{10};
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
                return (FloatArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
