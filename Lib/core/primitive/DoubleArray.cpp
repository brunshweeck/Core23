//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/private/Preconditions.h>
#include <core/IndexException.h>
#include "DoubleArray.h"

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

        DoubleArray::DoubleArray(gint length) : DoubleArray(length, false) {}

        DoubleArray::DoubleArray(gint length, gdouble initialValue) : Array<Double>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.DoubleArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_DOUBLE_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        DoubleArray::DoubleArray(const DoubleArray &array) : Array<Double>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.DoubleArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_DOUBLE_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        DoubleArray::DoubleArray(DoubleArray &&array) CORE_NOTHROW: Array<Double>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        DoubleArray &DoubleArray::operator=(const DoubleArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) Unsafe::U.reallocateMemory((glong) value,
                                                                 length * Unsafe::ARRAY_DOUBLE_INDEX_SCALE);
                    len = length;
                }
                for (int i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        DoubleArray &DoubleArray::operator=(DoubleArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        gdouble &DoubleArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Double>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.DoubleArray"));
            }
        }

        const gdouble DoubleArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Double>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.DoubleArray"));
            }
        }

        Object &DoubleArray::clone() const {
            return Unsafe::U.createInstance<DoubleArray>(*this);
        }

        DoubleArray::~DoubleArray() {
            Unsafe::U.freeMemory((glong) value);
            value = null;
        }
    } // core
} // primitive