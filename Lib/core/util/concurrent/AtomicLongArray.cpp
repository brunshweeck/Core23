//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include <core/IndexException.h>
#include <core/util/Preconditions.h>
#include <core/StringBuffer.h>
#include "AtomicLongArray.h"

namespace core {
    namespace util {
        glong AtomicLongArray::get(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getLongVolatile(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        void AtomicLongArray::set(gint i, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                U::putLongVolatile(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        String AtomicLongArray::toString() const {
            glong const iMax = array.length() - 1;
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

        void AtomicLongArray::lazySet(gint i, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                U::putLongRelease(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getAndSet(gint i, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndSetLong(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        gbool AtomicLongArray::compareAndSet(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::compareAndSetLong(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        gbool AtomicLongArray::weakCompareAndSetPlain(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::weakCompareAndSetLong(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getAndIncrement(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndAddLong(
                        null, addr,
                        1);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getAndDecrement(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndAddLong(
                        null, addr,
                        -1);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getAndAdd(gint i, glong delta) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndAddLong(
                        null, addr,
                        delta);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::incrementAndGet(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndAddLong(
                        null, addr,
                        1) + 1;
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::decrementAndGet(gint i) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndAddLong(
                        null, addr,
                        -1) - 1;
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::addAndGet(gint i, glong delta) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getAndAddLong(
                        null, addr,
                        delta) + delta;
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getPlain(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getLong(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        void AtomicLongArray::setPlain(gint i, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                U::putLong(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getOpaque(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getLongRelaxed(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        void AtomicLongArray::setOpaque(gint i, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                U::putLongRelaxed(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::getAcquire(gint i) const {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::getLongAcquire(
                        null, addr);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        void AtomicLongArray::setRelease(gint i, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                U::putLongRelease(
                        null, addr,
                        newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::compareAndExchange(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::compareAndExchangeLong(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::compareAndExchangeAcquire(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::compareAndExchangeLongAcquire(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        glong AtomicLongArray::compareAndExchangeRelease(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::compareAndExchangeLongRelease(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        gbool AtomicLongArray::weakCompareAndSetVolatile(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::weakCompareAndSetLong(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        gbool AtomicLongArray::weakCompareAndSetAcquire(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::weakCompareAndSetLongAcquire(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        gbool AtomicLongArray::weakCompareAndSetRelease(gint i, glong expectedValue, glong newValue) {
            try {
                glong const addr = U::getAddress(
                        array,
                        ((glong) Preconditions::checkIndex(i, array.length()) << 3) + U::ARRAY_LONG_BASE_OFFSET);
                return U::weakCompareAndSetLongRelease(
                        null, addr,
                        expectedValue, newValue);
            } catch (const IndexException &ie) {
                ie.throws(__trace("core.util.AtomicLongArray"));
            }
        }

        AtomicLongArray::AtomicLongArray(gint length) : array(length) {}

        AtomicLongArray::AtomicLongArray(const LongArray &array) : array(array) {}

        gint AtomicLongArray::length() const { return array.length(); }
    } // core

} // concurrent
