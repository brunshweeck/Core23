//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "ShortArray.h"

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

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_SHORT_INDEX_SCALE; }
        }

        ShortArray::ShortArray(gint length) : ShortArray(length, 0) {}

        ShortArray::ShortArray(gint length, gshort initialValue) : Array<Short>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ShortArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        ShortArray::ShortArray(const ShortArray &array) : Array<Short>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ShortArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        ShortArray::ShortArray(ShortArray &&array) CORE_NOTHROW: Array<Short>(0) {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        ShortArray &ShortArray::operator=(const ShortArray &array) {
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

        ShortArray &ShortArray::operator=(ShortArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gshort &ShortArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Short>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.ShortArray"));
            }
        }

        const gshort ShortArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Short>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.native.ShortArray"));
            }
        }

        Object &ShortArray::clone() const {
            return U::createInstance<ShortArray>(*this);
        }

        ShortArray::~ShortArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        ShortArray ShortArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ShortArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.ShortArray"));
            ShortArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }
    } // core
} // native

