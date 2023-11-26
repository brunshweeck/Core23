//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/IndexException.h>
#include <core/private/Preconditions.h>
#include "BooleanArray.h"

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

        BooleanArray::BooleanArray(gint length) : BooleanArray(length, false) {}

        BooleanArray::BooleanArray(gint length, gbool initialValue) : Array<Boolean>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.BooleanArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        BooleanArray::BooleanArray(const BooleanArray &array) : Array<Boolean>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.BooleanArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        BooleanArray::BooleanArray(BooleanArray &&array) CORE_NOTHROW: Array<Boolean>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        BooleanArray &BooleanArray::operator=(const BooleanArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) Unsafe::U.reallocateMemory((glong) value,
                                                                 length * Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
                    len = length;
                }
                for (int i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        BooleanArray &BooleanArray::operator=(BooleanArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        gbool &BooleanArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Boolean>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.BooleanArray"));
            }
        }

        const gbool BooleanArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Boolean>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.BooleanArray"));
            }
        }

        Object &BooleanArray::clone() const {
            return Unsafe::U.createInstance<BooleanArray>(*this);
        }

        BooleanArray::~BooleanArray() {
            Unsafe::U.freeMemory((glong) value);
            value = null;
        }
    } // core
} // primitive