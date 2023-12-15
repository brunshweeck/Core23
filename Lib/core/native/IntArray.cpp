//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "IntArray.h"

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

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_INT_INDEX_SCALE; }
        }

        IntArray::IntArray(gint length) : IntArray(length, 0) {}

        IntArray::IntArray(gint length, gint initialValue) : Array<Integer>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.IntArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        IntArray::IntArray(const IntArray &array) : Array<Integer>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.IntArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        IntArray::IntArray(IntArray &&array) CORE_NOTHROW: Array<Integer>(0) {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        IntArray &IntArray::operator=(const IntArray &array) {
            if (this != &array) {
                gint length = array.len;
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

        IntArray &IntArray::operator=(IntArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gint &IntArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Integer>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.IntArray"));
            }
        }

        const gint IntArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Integer>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.IntArray"));
            }
        }

        Object &IntArray::clone() const {
            return U::createInstance<IntArray>(*this);
        }

        IntArray::~IntArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        IntArray IntArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.IntArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.IntArray"));
            IntArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }
    } // core
} // native


