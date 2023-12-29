//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include "CharArray.h"

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

            CORE_FAST glong L(gint size) { return 1LL * size * U::ARRAY_CHAR_INDEX_SCALE; }
        }

        CharArray::CharArray(gint length) : CharArray(length, (gchar) 0) {}

        CharArray::CharArray(gint length, gchar initialValue) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.CharArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        CharArray::CharArray(const CharArray &array) {
            gint const length = array.length();
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.CharArray"));
            value = (STORAGE) U::allocateMemory(L(length));
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        CharArray::CharArray(CharArray &&array) CORE_NOTHROW {
            permute(value, array.value);
            permute(len, array.len);
            permute(isLocal, array.isLocal);
        }

        CharArray &CharArray::operator=(const CharArray &array) {
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

        CharArray &CharArray::operator=(CharArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
                permute(isLocal, array.isLocal);
            }
            return *this;
        }

        gchar &CharArray::get(gint index) {
            try {
                return value[Preconditions::checkIndex(index, len)];
            } catch (const IndexException &ie) { ie.throws(__trace("core.native.CharArray")); }
        }

        gchar CharArray::get(gint index) const {
            try {
                return value[Preconditions::checkIndex(index, len)];
            } catch (const IndexException &ie) { ie.throws(__trace("core.native.CharArray")); }
        }

        Object &CharArray::clone() const {
            return U::createInstance<CharArray>(*this);
        }

        CharArray::~CharArray() {
            len = 0;
            if (!isLocal)
                U::freeMemory((glong) value);
            value = null;
        }

        CharArray CharArray::fromAddress(glong addr, gint length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.CharArray"));
            if (addr == 0)
                ArgumentException("Null address").throws(__trace("core.native.CharArray"));
            CharArray ba = {};
            ba.value = (STORAGE) addr;
            ba.len = length;
            ba.isLocal = true;
            return ba;
        }

        gint CharArray::length() const {
            return len;
        }
    } // core
} // native

