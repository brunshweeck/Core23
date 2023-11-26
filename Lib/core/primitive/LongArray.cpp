//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/private/Preconditions.h>
#include <core/IndexException.h>
#include "LongArray.h"

namespace core {
    namespace primitive {

        using native::Unsafe;
        using util::Preconditions;

        namespace {
            template<class T, class K>
            void permute(T &t, K &k) {
                T t2 = t;
                t = k;
                k = t2;
            }
        }

        LongArray::LongArray(gint length) : LongArray(length, false) {}

        LongArray::LongArray(gint length, glong initialValue) : Array<Long>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.LongArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_LONG_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        LongArray::LongArray(const LongArray &array) : Array<Long>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.LongArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_LONG_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        LongArray::LongArray(LongArray &&array) CORE_NOTHROW: Array<Long>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        LongArray &LongArray::operator=(const LongArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) Unsafe::U.reallocateMemory((glong) value,
                                                                 length * Unsafe::ARRAY_LONG_INDEX_SCALE);
                    len = length;
                }
                for (int i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        LongArray &LongArray::operator=(LongArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        glong &LongArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Long>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.LongArray"));
            }
        }

        const glong LongArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Long>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.LongArray"));
            }
        }

        Object &LongArray::clone() const {
            return Unsafe::U.createInstance<LongArray>(*this);
        }

        LongArray::~LongArray() {
            Unsafe::U.freeMemory((glong) value);
            value = null;
        }
    } // core
} // primitive