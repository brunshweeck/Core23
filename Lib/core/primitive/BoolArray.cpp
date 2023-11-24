//
// Created by T.N.Brunshweeck on 19/11/2023.
//

#include <core/private/Unsafe.h>
#include <core/ArgumentException.h>
#include <core/IndexException.h>
#include <core/private/Preconditions.h>
#include "BoolArray.h"

namespace core {
    namespace primitive {

        using native::Unsafe;
        using util::Preconditions;

        BoolArray::BoolArray(gint length) : BoolArray(length, false) {}

        BoolArray::BoolArray(gint length, gbool initialValue) : Array<Boolean>(length) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("Core::BoolArray"));
            value = (STORAGE) Unsafe::U.allocateMemory(length * (glong) Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
            Unsafe::U.setMemory(*this, offsetof(BoolArray, value), length * Unsafe::ARRAY_BOOLEAN_INDEX_SCALE,
                                initialValue ? 1 : 0);
        }

        BoolArray::BoolArray(const BoolArray &array) : Array<Boolean>(array) {
            gint len = array.length();
            if (len > 0) {
                value = (STORAGE) Unsafe::U.allocateMemory(len * (glong) Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
                Unsafe::U.copyMemory(
                        array, offsetof(BoolArray, value),
                        *this, offsetof(BoolArray, value),
                        len * (glong) Unsafe::ARRAY_BOOLEAN_INDEX_SCALE);
            }
        }

        BoolArray::BoolArray(BoolArray &&array) CORE_NOTHROW:
                Array<Boolean>(Unsafe::forwardInstance<BoolArray>(array)) {
            value = array.value;
            array.value = null;
        }

        BoolArray &BoolArray::operator=(const BoolArray &array) {
            if (this != &array) {
                CORE_CAST(Array<Boolean> &, *this) = CORE_CAST(Array<Boolean> &, array);

            }
            return *this;
        }

        BoolArray &BoolArray::operator=(BoolArray &&array) CORE_NOTHROW {
            if (this != &array) {
                //
            }
            return *this;
        }

        gbool &BoolArray::get(gint index) {
            try {
                Preconditions::checkIndex(index, Array<Boolean>::length());
            } catch (const IndexException &ie) {
                ie.throws(__trace("Core::BoolArray"));
            }
            return value[index];
        }

        const gbool BoolArray::get(gint index) const {
            try {
                Preconditions::checkIndex(index, Array<Boolean>::length());
            } catch (const IndexException &ie) {
                ie.throws(__trace("Core::BoolArray"));
            }
            return value[index];
        }

        Object &BoolArray::clone() const {
            return Unsafe::U.createInstance<BoolArray>(*this);
        }

        BoolArray::~BoolArray() {
            Unsafe::U.freeMemory((glong) value);
            value = null;
        }
    } // core
} // primitive