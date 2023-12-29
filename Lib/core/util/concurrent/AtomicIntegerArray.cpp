//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include <core/IndexException.h>
#include <core/util/Preconditions.h>
#include <core/StringBuffer.h>
#include "AtomicIntegerArray.h"

namespace core {
    namespace util {
        gint AtomicIntegerArray::get(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getIntVolatile(null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        void AtomicIntegerArray::set(gint i, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                U::putIntVolatile(null, addr, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        String AtomicIntegerArray::toString() const {
            gint const iMax = array.length() - 1;
            if (iMax == -1)
                return "[]";

            StringBuffer b = StringBuffer();
            b.append('[');
            for (int i = 0;; i++) {
                b.append(get(i));
                if (i == iMax)
                    return b.append(']').toString();
                b.append(',').append(' ');
            }
        }

        void AtomicIntegerArray::lazySet(gint i, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                U::putIntRelease(null, addr, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getAndSet(gint i, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndSetInt(null, addr, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gbool AtomicIntegerArray::compareAndSet(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::compareAndSetInt(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gbool AtomicIntegerArray::weakCompareAndSetPlain(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::weakCompareAndSetInt(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getAndIncrement(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndAddInt(
                        null, addr,
                        1);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getAndDecrement(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndAddInt(
                        null, addr,
                        -1);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getAndAdd(gint i, gint delta) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndAddInt(
                        null, addr,
                        delta);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::incrementAndGet(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndAddInt(
                        null, addr,
                        1) + 1;
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::decrementAndGet(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndAddInt(
                        null, addr,
                        -1) - 1;
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::addAndGet(gint i, gint delta) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getAndAddInt(
                        null, addr,
                        delta) + delta;
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getPlain(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getInt(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        void AtomicIntegerArray::setPlain(gint i, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                U::putInt(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getOpaque(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getIntRelaxed(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        void AtomicIntegerArray::setOpaque(gint i, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                U::putIntRelaxed(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::getAcquire(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::getIntAcquire(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        void AtomicIntegerArray::setRelease(gint i, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                U::putIntRelease(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::compareAndExchange(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::compareAndExchangeInt(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::compareAndExchangeAcquire(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::compareAndExchangeIntAcquire(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gint AtomicIntegerArray::compareAndExchangeRelease(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::compareAndExchangeIntRelease(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gbool AtomicIntegerArray::weakCompareAndSetVolatile(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::weakCompareAndSetInt(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gbool AtomicIntegerArray::weakCompareAndSetAcquire(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::weakCompareAndSetIntAcquire(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        gbool AtomicIntegerArray::weakCompareAndSetRelease(gint i, gint expectedValue, gint newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 2) + U::ARRAY_INT_BASE_OFFSET);
                return U::weakCompareAndSetIntRelease(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicIntegerArray"));
            }
        }

        AtomicIntegerArray::AtomicIntegerArray(gint length) : array(length) {}

        AtomicIntegerArray::AtomicIntegerArray(const IntArray &array) : array(array) {}

        gint AtomicIntegerArray::length() const { return array.length(); }
    } // core
} // concurrent