//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicLong.h"
#include <core/Long.h>

namespace core {
    namespace util {
        String AtomicLong::toString() const { return Long::toString(get()); }
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicLong::VALUE = CORE_OFFSETOF(AtomicLong, value)

        glong AtomicLong::get() const { return value; }

        void AtomicLong::set(glong newValue) { value = newValue; }

        void AtomicLong::lazySet(glong newValue) { U::putLongRelease(*this, VALUE, newValue); }

        glong AtomicLong::getAndSet(glong newValue) { return U::getAndSetLong(*this, VALUE, newValue); }

        gbool AtomicLong::compareAndSet(glong expectedValue, glong newValue) {
            return U::compareAndSetLong(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSet(glong expectedValue, glong newValue) {
            return U::weakCompareAndSetLong(*this, VALUE, expectedValue, newValue);
        }

        glong AtomicLong::getAndIncrement() { return U::getAndAddLong(*this, VALUE, 1); }

        glong AtomicLong::getAndDecrement() { return U::getAndAddLong(*this, VALUE, -1); }

        glong AtomicLong::getAndAdd(glong delta) { return U::getAndAddLong(*this, VALUE, delta); }

        glong AtomicLong::incrementAndGet() { return U::getAndAddLong(*this, VALUE, 1) + 1; }

        glong AtomicLong::decrementAndGet() { return U::getAndAddLong(*this, VALUE, -1) - 1; }

        glong AtomicLong::addAndGet(glong delta) { return U::getAndAddLong(*this, VALUE, delta) + delta; }

        glong AtomicLong::intValue() const { return (gint) get(); }

        glong AtomicLong::longValue() const { return get(); }

        gfloat AtomicLong::floatValue() const { return (gfloat) get(); }

        gdouble AtomicLong::doubleValue() const { return (gdouble) get(); }

        glong AtomicLong::getPlain() const { return U::getLong(*this, VALUE); }

        void AtomicLong::setPlain(glong newValue) { U::putLong(*this, VALUE, newValue); }

        glong AtomicLong::getOpaque() const { return U::getLongRelaxed(*this, VALUE); }

        void AtomicLong::setOpaque(glong newValue) { U::putLongRelaxed(*this, VALUE, newValue); }

        glong AtomicLong::getAcquire() const { return U::getLongAcquire(*this, VALUE); }

        void AtomicLong::setRelease(glong newValue) { U::putLongRelease(*this, VALUE, newValue); }

        glong AtomicLong::compareAndExchange(glong expectedValue, glong newValue) {
            return U::compareAndExchangeLong(*this, VALUE, expectedValue, newValue);
        }

        glong AtomicLong::compareAndExchangeAcquire(glong expectedValue, glong newValue) {
            return U::compareAndExchangeLongAcquire(*this, VALUE, expectedValue, newValue);
        }

        glong AtomicLong::compareAndExchangeRelease(glong expectedValue, glong newValue) {
            return U::compareAndExchangeLongRelease(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetVolatile(glong expectedValue, glong newValue) {
            return U::weakCompareAndSetLong(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetAcquire(glong expectedValue, glong newValue) {
            return U::weakCompareAndSetLongAcquire(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetRelease(glong expectedValue, glong newValue) {
            return U::weakCompareAndSetLongRelease(*this, VALUE, expectedValue, newValue);
        };
        CORE_WARNING_POP
    } // core
} // concurrent