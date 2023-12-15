//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "FloatArray.h"

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

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_FLOAT_INDEX_SCALE; }
        }

        FloatArray::FloatArray(gint length) : FloatArray(length, 0.0F) {}

        FloatArray::FloatArray(gint length, gfloat initialValue) : Array<Float>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.FloatArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        FloatArray::FloatArray(const FloatArray &array) : Array<Float>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.FloatArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        FloatArray::FloatArray(FloatArray &&array) CORE_NOTHROW: Array<Float>(0) {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        FloatArray &FloatArray::operator=(const FloatArray &array) {
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

        FloatArray &FloatArray::operator=(FloatArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gfloat &FloatArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Float>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.FloatArray"));
            }
        }

        const gfloat FloatArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Float>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.FloatArray"));
            }
        }

        Object &FloatArray::clone() const {
            return U::createInstance<FloatArray>(*this);
        }

        FloatArray::~FloatArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        FloatArray FloatArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.FloatArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.FloatArray"));
            FloatArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }
    } // core
} // native

