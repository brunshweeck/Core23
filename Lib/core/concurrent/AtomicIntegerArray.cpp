//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicIntegerArray.h"
#include <core/IndexException.h>
#include <core/StringBuffer.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace concurrent {

        using namespace util;
        using namespace native;

        gint AtomicIntegerArray::get(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getIntVolatile(array, offset);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        void AtomicIntegerArray::set(gint i, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 Unsafe::putIntVolatile(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        String AtomicIntegerArray::toString() const {
            gint const iMax = array.length() - 1;
            if (iMax == -1)
                return "[]";

            StringBuffer b = StringBuffer();
            b.append('[');
            for (gint i = 0;; i++) {
                b.append(get(i));
                if (i == iMax)
                    return b.append(']').toString();
                b.append(',').append(' ');
            }
        }

        void AtomicIntegerArray::lazySet(gint i, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 Unsafe::putIntRelease(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getAndSet(gint i, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndSetInt(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gbool AtomicIntegerArray::compareAndSet(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::compareAndSetInt(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gbool AtomicIntegerArray::weakCompareAndSetPlain(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetInt(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getAndIncrement(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndAddInt(array, offset, 1);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getAndDecrement(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndAddInt(array, offset, -1);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getAndAdd(gint i, gint delta) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndAddInt(array, offset, delta);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::incrementAndGet(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndAddInt(array, offset, 1) + 1;
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::decrementAndGet(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndAddInt(array, offset, -1) - 1;
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::addAndGet(gint i, gint delta) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getAndAddInt(array, offset, delta) + delta;
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getPlain(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getInt(array, offset);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        void AtomicIntegerArray::setPlain(gint i, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 Unsafe::putInt(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getOpaque(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getIntRelaxed(array, offset);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        void AtomicIntegerArray::setOpaque(gint i, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 Unsafe::putIntRelaxed(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::getAcquire(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::getIntAcquire(array, offset);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        void AtomicIntegerArray::setRelease(gint i, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 Unsafe::putIntRelease(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::compareAndExchange(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::compareAndExchangeInt(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::compareAndExchangeAcquire(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::compareAndExchangeIntAcquire(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gint AtomicIntegerArray::compareAndExchangeRelease(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::compareAndExchangeIntRelease(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gbool AtomicIntegerArray::weakCompareAndSetVolatile(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetInt(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gbool AtomicIntegerArray::weakCompareAndSetAcquire(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetIntAcquire(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        gbool AtomicIntegerArray::weakCompareAndSetRelease(gint i, gint expectedValue, gint newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_INT_BASE_OFFSET + (i * Unsafe::ARRAY_INT_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetIntRelease(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicIntegerArray"));
        }

        AtomicIntegerArray::AtomicIntegerArray(gint length) : array(length) {}

        AtomicIntegerArray::AtomicIntegerArray(const IntArray &array) : array(array) {}

        gint AtomicIntegerArray::length() const { return array.length(); }
    } // core
} // concurrent
