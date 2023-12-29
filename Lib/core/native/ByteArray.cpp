//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "ByteArray.h"

namespace core {
    namespace native {

        CORE_ALIAS(U, native::Unsafe);
        using util::Preconditions;

        namespace {
            template<class T, class K>
            void permute(T &t, K &k) {
                T t2 = t;
                t = k;
                k = t2;
            }

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_BYTE_INDEX_SCALE; }
        }

        ByteArray::ByteArray(gint length) : ByteArray(length, (gbyte) 0) {}

        ByteArray::ByteArray(gint length, gbyte initialValue) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ByteArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        ByteArray::ByteArray(const ByteArray &array) {
            gint const length = array.length();
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ByteArray"));
            value = (STORAGE) U::allocateMemory(L(len = length));
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        ByteArray::ByteArray(ByteArray &&array) CORE_NOTHROW {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        ByteArray &ByteArray::operator=(const ByteArray &array) {
            if (this != &array) {
                gint const length = array.len;
                if (array.isLocal) {
                    if (!isLocal) {
                        U::freeMemory((glong) value);
                    }
                    value = array.value;
                    len = array.len;
                    isLocal = true;
                } else {
                    if (len != length) {
                        STORAGE newValue = (STORAGE) U::allocateMemory(L(length));
                        if (!isLocal) {
                            U::freeMemory((glong) value);
                            value = newValue;
                        }
                        len = length;
                    }
                    U::copySwapMemory((glong) array.value, (glong) value, L(length), 1);
                }
            }
            return *this;
        }

        ByteArray &ByteArray::operator=(ByteArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gbyte &ByteArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.ByteArray"));
            }
        }

        gbyte ByteArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.ByteArray"));
            }
        }

        Object &ByteArray::clone() const {
            return U::createInstance<ByteArray>(*this);
        }

        ByteArray::~ByteArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        ByteArray ByteArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ByteArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.ByteArray"));
            ByteArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }

        gint ByteArray::length() const {
            return len;
        }
    } // core
} // native

