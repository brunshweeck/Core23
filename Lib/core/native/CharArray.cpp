//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include "CharArray.h"
#include <core/private/Unsafe.h>
#include <core/IllegalArgumentException.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace native {
        using namespace util;

        CharArray::CharArray() : len(0), value(null) {}

        CharArray::CharArray(gint length) : CharArray(length, u'\0') {}

        CharArray::CharArray(gint length, CharArray::Value initialValue) : CharArray() {
            if (length < 0)
                IllegalArgumentException("Negative array length").throws(__trace("core.native.CharArray"));
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

        CharArray::CharArray(const CharArray &array) : CharArray(array.len) {
            // assert len == array.len
            gint const n = array.len;
            for (int i = 0; i < n; i += 1) {
                value[i] = array.value[i];
            }
        }

        CharArray::CharArray(CharArray &&array) CORE_NOTHROW: CharArray() {
            Unsafe::swapValues(value, array.value);
            Unsafe::swapValues(len, array.len);
        }

        CharArray &CharArray::operator=(const CharArray &array) {
            if (this != &array) {
                CharArray copy{array};
                Unsafe::swapValues(value, copy.value);
                Unsafe::swapValues(len, copy.len);
            }
            return *this;
        }

        CharArray &CharArray::operator=(CharArray &&array) CORE_NOTHROW {
            if (this != &array) {
                Unsafe::swapValues(value, array.value);
                Unsafe::swapValues(len, array.len);
            }
            return *this;
        }

        gint CharArray::length() const {
            return Math::max(len, 0);
        }

        gbool CharArray::isEmpty() const {
            return len <= 0 || value == null;
        }

        CharArray::Value &CharArray::get(gint index) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray"));
            }
        }

        const CharArray::Value &CharArray::get(gint index) const {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                return value[i];
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray"));
            }
        }

        void CharArray::set(gint index, const CharArray::Value &newValue) {
            try{
                gint const n = length();
                gint const i = Preconditions::checkIndex(index, n);
                value[i] = newValue;
            } catch (const Exception &ex) {
                ex.throws(__trace("core.native.CharArray"));
            }
        }

        Object &CharArray::clone() const {
            return Unsafe::allocateInstance<CharArray>(*this);
        }

        CharArray::~CharArray() {
            len = 0;
            Unsafe::freeMemory((glong) value);
            value = null;
        }

        gbool CharArray::equals(const Object &o) const {
            if (this == &o) {
                return true;
            }
            if (!Class<PrimitiveArray<Character>>::hasInstance(o)) {
                return false;
            }
            PrimitiveArray<Character> const& array = (PrimitiveArray<Character> const&) o;
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

        CharArray CharArray::of() {
            return {};
        }

        CharArray CharArray::of(CharArray::Value v0) {
            try{
                CharArray array{1};
                array.value[0] = v0;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray CharArray::of(CharArray::Value v0, CharArray::Value v1) {
            try{
                CharArray array{2};
                array.value[0] = v0;
                array.value[1] = v1;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2) {
            try{
                CharArray array{3};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2,
                                      CharArray::Value v3) {
            try{
                CharArray array{4};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray
        CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2, CharArray::Value v3,
                         CharArray::Value v4) {
            try{
                CharArray array{5};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray
        CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2, CharArray::Value v3,
                         CharArray::Value v4, CharArray::Value v5) {
            try{
                CharArray array{6};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray
        CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2, CharArray::Value v3,
                         CharArray::Value v4, CharArray::Value v5, CharArray::Value v6) {
            try{
                CharArray array{7};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray
        CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2, CharArray::Value v3,
                         CharArray::Value v4, CharArray::Value v5, CharArray::Value v6,
                         CharArray::Value v7) {
            try{
                CharArray array{8};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray
        CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2, CharArray::Value v3,
                         CharArray::Value v4, CharArray::Value v5, CharArray::Value v6, CharArray::Value v7,
                         CharArray::Value v8) {
            try{
                CharArray array{9};
                array.value[0] = v0;
                array.value[1] = v1;
                array.value[2] = v2;
                array.value[3] = v3;
                array.value[4] = v4;
                array.value[5] = v5;
                array.value[6] = v6;
                array.value[7] = v7;
                array.value[8] = v8;
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

        CharArray
        CharArray::of(CharArray::Value v0, CharArray::Value v1, CharArray::Value v2, CharArray::Value v3,
                         CharArray::Value v4, CharArray::Value v5, CharArray::Value v6, CharArray::Value v7,
                         CharArray::Value v8, CharArray::Value v9) {
            try{
                CharArray array{10};
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
                return (CharArray &&) array;
            } catch (const MemoryError &error) {
                error.throws();
            }
            return {};
        }

    } // core
} // native
