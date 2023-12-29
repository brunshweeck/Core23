//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/IndexException.h>
#include <core/util/Preconditions.h>
#include "BooleanArray.h"

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

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_BOOLEAN_INDEX_SCALE; }
        }

        BooleanArray::BooleanArray(gint length) : BooleanArray(length, false) {}

        BooleanArray::BooleanArray(gint length, gbool initialValue) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.BooleanArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        BooleanArray::BooleanArray(const BooleanArray &array) {
            gint const length = array.length();
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.BooleanArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        BooleanArray::BooleanArray(BooleanArray &&array) CORE_NOTHROW {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        BooleanArray &BooleanArray::operator=(const BooleanArray &array) {
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
                    U::copySwapMemory((glong) array.value, (glong)value, L(length), 1);
                }
            }
            return *this;
        }

        BooleanArray &BooleanArray::operator=(BooleanArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gbool &BooleanArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.BooleanArray"));
            }
        }

        gbool BooleanArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, len);
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.BooleanArray"));
            }
        }

        Object &BooleanArray::clone() const {
            return U::createInstance<BooleanArray>(*this);
        }

        BooleanArray::~BooleanArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        BooleanArray BooleanArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.BooleanArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.BooleanArray"));
            BooleanArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }

        gint BooleanArray::length() const {
            return len;
        }
    } // core
} // native