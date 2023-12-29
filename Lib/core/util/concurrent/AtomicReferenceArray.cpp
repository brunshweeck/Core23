//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include <core/util/Preconditions.h>
#include <core/IndexException.h>
#include <core/StringBuffer.h>
#include "AtomicReferenceArray.h"

namespace core {
    namespace util {

        Object &AtomicReferenceArray::get(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::getReferenceVolatile(null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        void AtomicReferenceArray::set(gint i, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                U::putReferenceVolatile(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        String AtomicReferenceArray::toString() const {
            gint const iMax = array.length() - 1;
            if (iMax == -1)
                return "[]";

            StringBuffer b = {};
            b.append('[');
            for (int i = 0;; i++) {
                b.append(get(i));
                if (i == iMax)
                    return b.append(']').toString();
                b.append(',').append(' ');
            }
        }

        void AtomicReferenceArray::lazySet(gint i, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                U::putReferenceRelease(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::getAndSet(gint i, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::getAndSetReference(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        gbool AtomicReferenceArray::compareAndSet(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::compareAndSetReference(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        gbool AtomicReferenceArray::weakCompareAndSetPlain(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::weakCompareAndSetReference(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::getPlain(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::getReference(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        void AtomicReferenceArray::setPlain(gint i, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                U::putReference(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::getOpaque(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::getReferenceRelaxed(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        void AtomicReferenceArray::setOpaque(gint i, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                U::putReferenceRelaxed(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::getAcquire(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::getReferenceAcquire(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        void AtomicReferenceArray::setRelease(gint i, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                U::putReferenceRelease(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::compareAndExchange(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::compareAndExchangeReference(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::compareAndExchangeAcquire(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::compareAndExchangeReferenceAcquire(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        Object &AtomicReferenceArray::compareAndExchangeRelease(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::compareAndExchangeReferenceRelease(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        gbool AtomicReferenceArray::weakCompareAndSetVolatile(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::weakCompareAndSetReference(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        gbool AtomicReferenceArray::weakCompareAndSetAcquire(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::weakCompareAndSetReferenceAcquire(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        gbool AtomicReferenceArray::weakCompareAndSetRelease(gint i, const Object &expectedValue, Object &newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) * U::ARRAY_REFERENCE_INDEX_SCALE) +
                        U::ARRAY_REFERENCE_BASE_OFFSET);
                return U::weakCompareAndSetReferenceRelease(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicReferenceArray"));
            }
        }

        gint AtomicReferenceArray::length() const { return array.length(); }

        AtomicReferenceArray::AtomicReferenceArray(gint length) : array(length) {}

        AtomicReferenceArray::AtomicReferenceArray(const ReferenceArray &array) : array(array) {}

    } // core
} // concurrent