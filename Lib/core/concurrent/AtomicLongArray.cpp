//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicLongArray.h"
#include <core/IndexException.h>
#include <core/StringBuffer.h>
#include <core/util/Preconditions.h>

namespace core {
    namespace concurrent {

        using namespace util;
        using namespace native;

        glong AtomicLongArray::get(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getLongVolatile(array, offset);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        void AtomicLongArray::set(gint i, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 Unsafe::putLongVolatile(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        String AtomicLongArray::toString() const {
            glong const iMax = array.length() - 1;
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

        void AtomicLongArray::lazySet(gint i, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 Unsafe::putLongRelease(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getAndSet(gint i, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndSetLong(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        gbool AtomicLongArray::compareAndSet(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::compareAndSetLong(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        gbool AtomicLongArray::weakCompareAndSetPlain(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetLong(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getAndIncrement(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndAddLong(array, offset, 1L);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getAndDecrement(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndAddLong(array, offset, -1L);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getAndAdd(gint i, glong delta) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndAddLong(array, offset, delta);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::incrementAndGet(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndAddLong(array, offset, 1L) + 1L;
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::decrementAndGet(gint i) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndAddLong(array, offset, -1) - 1;
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::addAndGet(gint i, glong delta) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getAndAddLong(array, offset, delta) + delta;
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getPlain(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getLong(array, offset);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        void AtomicLongArray::setPlain(gint i, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 Unsafe::putLong(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getOpaque(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getLongRelaxed(array, offset);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        void AtomicLongArray::setOpaque(gint i, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 Unsafe::putLongRelaxed(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::getAcquire(gint i) const {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::getLongAcquire(array, offset);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        void AtomicLongArray::setRelease(gint i, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 Unsafe::putLongRelease(array, offset, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::compareAndExchange(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::compareAndExchangeLong(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::compareAndExchangeAcquire(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::compareAndExchangeLongAcquire(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        glong AtomicLongArray::compareAndExchangeRelease(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::compareAndExchangeLongRelease(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        gbool AtomicLongArray::weakCompareAndSetVolatile(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetLong(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        gbool AtomicLongArray::weakCompareAndSetAcquire(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetLongAcquire(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        gbool AtomicLongArray::weakCompareAndSetRelease(gint i, glong expectedValue, glong newValue) {
            CORE_TRY_RETHROW_EXCEPTION
            ({
                 Preconditions::checkIndex(i, array.length());
                 glong const offset = Unsafe::ARRAY_LONG_BASE_OFFSET + (i * Unsafe::ARRAY_LONG_INDEX_SCALE);
                 return Unsafe::weakCompareAndSetLongRelease(array, offset, expectedValue, newValue);
             }, , __trace("core.concurrent.AtomicLongArray"));
        }

        AtomicLongArray::AtomicLongArray(gint length) : array(length) {}

        AtomicLongArray::AtomicLongArray(const LongArray &array) : array(array) {}

        gint AtomicLongArray::length() const { return array.length(); }
    } // core

} // concurrent
