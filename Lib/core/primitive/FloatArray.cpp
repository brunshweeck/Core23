//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/private/Preconditions.h>
#include <core/IndexException.h>
#include "FloatArray.h"

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

        FloatArray::FloatArray(gint length) : FloatArray(length, false) {}

        FloatArray::FloatArray(gint length, gfloat initialValue) : Array<Float>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.FloatArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_FLOAT_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = initialValue;
        }

        FloatArray::FloatArray(const FloatArray &array) : Array<Float>(0) {
            gint length = array.length();
            if (len < 0)
                ArgumentException("Negative array length").throws(__trace("core.FloatArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_FLOAT_INDEX_SCALE);
            len = length;
            for (int i = 0; i < length; ++i)
                value[i] = array.value[i];
        }

        FloatArray::FloatArray(FloatArray &&array) CORE_NOTHROW: Array<Float>(0) {
            permute(value, array.value);
            permute(len, array.len);
        }

        FloatArray &FloatArray::operator=(const FloatArray &array) {
            if (this != &array) {
                gint length = array.len;
                if (len != length) {
                    value = (STORAGE) Unsafe::U.reallocateMemory((glong) value,
                                                                 length * Unsafe::ARRAY_FLOAT_INDEX_SCALE);
                    len = length;
                }
                for (int i = 0; i < length; ++i)
                    value[i] = array.value[i];
            }
            return *this;
        }

        FloatArray &FloatArray::operator=(FloatArray &&array) CORE_NOTHROW {
            if (this != &array) {
                permute(value, array.value);
                permute(len, array.len);
            }
            return *this;
        }

        gfloat &FloatArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Float>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.FloatArray"));
            }
        }

        const gfloat FloatArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Float>::length());
                return value[index];
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.FloatArray"));
            }
        }

        Object &FloatArray::clone() const {
            return Unsafe::U.createInstance<FloatArray>(*this);
        }

        FloatArray::~FloatArray() {
            Unsafe::U.freeMemory((glong) value);
            value = null;
        }
    } // core
} // primitive