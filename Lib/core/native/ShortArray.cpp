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
        }

        ShortArray::ShortArray(gint length) : ShortArray(length, false) {}

        ShortArray::ShortArray(gint length, gshort initialValue) : Array<Short>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.ShortArray"));
            value = (STORAGE) U::allocateMemory(length * (glong) U::ARRAY_SHORT_INDEX_SCALE);
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        ShortArray::ShortArray(const ShortArray &array) : Array<Short>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.ShortArray"));
            value = (STORAGE) U::allocateMemory(length * (glong) U::ARRAY_SHORT_INDEX_SCALE);
            len = length;
            for (gint i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        ShortArray::ShortArray(ShortArray &&array) CORE_NOTHROW: Array<Short>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        ShortArray &ShortArray::operator=(const ShortArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) U::reallocateMemory((glong) value,
                                                                 length * U::ARRAY_SHORT_INDEX_SCALE);
                    len = length;
                }
                for (gint i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        ShortArray &ShortArray::operator=(ShortArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        gshort &ShortArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Short>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.ShortArray"));
            }
        }

        const gshort ShortArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Short>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.ShortArray"));
            }
        }

        Object &ShortArray::clone() const {
            return U::createInstance<ShortArray>(*this);
        }

        ShortArray::~ShortArray() {
            U::freeMemory((glong) value);
            value = null;
        }
    } // core
} // native