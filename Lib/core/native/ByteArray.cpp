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
        }

        ByteArray::ByteArray(gint length) : ByteArray(length, false) {}

        ByteArray::ByteArray(gint length, gbyte initialValue) : Array<Byte>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.ByteArray"));
            value = (STORAGE) U::allocateMemory(length * (glong) U::ARRAY_BYTE_INDEX_SCALE);
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        ByteArray::ByteArray(const ByteArray &array) : Array<Byte>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.ByteArray"));
            value = (STORAGE) U::allocateMemory(length * (glong) U::ARRAY_BYTE_INDEX_SCALE);
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        ByteArray::ByteArray(ByteArray &&array) CORE_NOTHROW: Array<Byte>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        ByteArray &ByteArray::operator=(const ByteArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) U::reallocateMemory((glong) value,
                                                                 length * U::ARRAY_BYTE_INDEX_SCALE);
                    len = length;
                }
                for (gint i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        ByteArray &ByteArray::operator=(ByteArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        gbyte &ByteArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Byte>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.ByteArray"));
            }
        }

        const gbyte ByteArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Byte>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.ByteArray"));
            }
        }

        Object &ByteArray::clone() const {
            return U::createInstance<ByteArray>(*this);
        }

        ByteArray::~ByteArray() {
            U::freeMemory((glong) value);
            value = null;
        }
    } // core
} // native