//
// Created by T.N.Brunshweeck on 16/11/2023.
//

#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include <core/StringBuffer.h>
#include <core/private/Unsafe.h>
#include "ReferenceArray.h"

namespace core {
    namespace native {

        using util::Preconditions;
        CORE_ALIAS(U, native::Unsafe);

        namespace {
            CORE_FAST gint const Shift = sizeof(void *) == 8 ? 3 : 2;

            CORE_FAST glong L(gint length) { return ((glong) length) << Shift; }

            glong o2l(const Object &o) { return null == o ? 0 : (glong) &o; }

            template<class P>
            glong p2l(P p) { return (glong) p; }
        }

        ReferenceArray::ReferenceArray(gint length) : ReferenceArray(length, null) {}

        ReferenceArray::ReferenceArray(gint length, Object &initialValue) {
            if (length < 0)
                ArgumentException("Negative array length").throws(__trace("core.native.ReferenceArray"));
            value = (STORAGE) U::allocateMemory((len = length) << Shift);
            for (int i = 0; i < length; ++i)
                value[i] = null == initialValue ? null : &initialValue;
        }

        ReferenceArray::ReferenceArray(const ReferenceArray &a) {
            value = (STORAGE) U::allocateMemory(L(len = a.len));
            U::copyMemory(p2l(a.value), p2l(value), len << Shift);
        }

        ReferenceArray::ReferenceArray(ReferenceArray &&a) CORE_NOTHROW {
            U::swapValues(value, a.value);
            U::swapValues(len, a.len);
        }

        ReferenceArray &ReferenceArray::operator=(const ReferenceArray &a) {
            if (this != &a) {
                gint const aSize = a.len;
                if (len != aSize) {
                    STORAGE newData = (STORAGE) U::allocateMemory(L(aSize));
                    U::swapValues(newData, value);
                    U::freeMemory((glong) newData);
                    len = aSize;
                }
                U::copyMemory(p2l(a.value), p2l(value), L(aSize));
            }
            return *this;
        }

        ReferenceArray &ReferenceArray::operator=(ReferenceArray &&a) CORE_NOTHROW {
            if (this != &a) {
                U::swapValues(value, a.value);
                U::swapValues(len, a.len);
            }
            return *this;
        }

        Object &ReferenceArray::clone() const {
            return U::createInstance<ReferenceArray>(*this);
        }

        String ReferenceArray::toString() const {
            if (isEmpty())
                return "[]";
            gint const aSize = len;
            StringBuffer sb = {};
            sb.append('[');
            for (int i = 0; i < aSize; ++i) {
                sb.append(get(i));
                if (i < aSize - 1)
                    sb.append(", ");
            }
            sb.append(']');
            return sb.toString();
        }

        Object &ReferenceArray::get(gint index) {
            try {
                VRef r = value[Preconditions::checkIndex(index, len)];
                return r == null ? null : *r;
            } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
        }

        const Object &ReferenceArray::get(gint index) const {
            try {
                VRef r = value[Preconditions::checkIndex(index, len)];
                return r == null ? null : *r;
            } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
        }

        void ReferenceArray::set(gint index, Object &newValue) {
            try {
                value[Preconditions::checkIndex(index, len)] = null == newValue ? null : &newValue;
            } catch (const IndexException &ie) { ie.throws(__trace("core.native.ReferenceArray")); }
        }

        ReferenceArray::~ReferenceArray() {
            len = 0;
            U::freeMemory((glong) value);
            value = null;
        }

        gint ReferenceArray::length() const { return len; }
    } // core
} // native