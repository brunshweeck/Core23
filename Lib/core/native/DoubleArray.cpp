//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "DoubleArray.h"

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

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_DOUBLE_INDEX_SCALE; }
        }

        DoubleArray::DoubleArray(gint length) : DoubleArray(length, 0.0) {}

        DoubleArray::DoubleArray(gint length, gdouble initialValue) : Array<Double>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.DoubleArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        DoubleArray::DoubleArray(const DoubleArray &array) : Array<Double>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.DoubleArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        DoubleArray::DoubleArray(DoubleArray &&array) CORE_NOTHROW: Array<Double>(0) {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        DoubleArray &DoubleArray::operator=(const DoubleArray &array) {
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

        DoubleArray &DoubleArray::operator=(DoubleArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gdouble &DoubleArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Double>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.DoubleArray"));
            }
        }

        const gdouble DoubleArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Double>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.DoubleArray"));
            }
        }

        Object &DoubleArray::clone() const {
            return U::createInstance<DoubleArray>(*this);
        }

        DoubleArray::~DoubleArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        DoubleArray DoubleArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.DoubleArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.DoubleArray"));
            DoubleArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }
    } // core
} // native

