//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicLong.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent {

        using namespace native;

        String AtomicLong::toString() const { return Long::toString(get()); }
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicLong::VALUE = CORE_OFFSETOF(AtomicLong, value)

        glong AtomicLong::get() const { return value; }

        void AtomicLong::set(glong newValue) { value = newValue; }

        void AtomicLong::lazySet(glong newValue) { Unsafe::putLongRelease(*this, VALUE, newValue); }

        glong AtomicLong::getAndSet(glong newValue) { return Unsafe::getAndSetLong(*this, VALUE, newValue); }

        gbool AtomicLong::compareAndSet(glong expectedValue, glong newValue) {
            return Unsafe::compareAndSetLong(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSet(glong expectedValue, glong newValue) {
            return Unsafe::weakCompareAndSetLong(*this, VALUE, expectedValue, newValue);
        }

        glong AtomicLong::getAndIncrement() { return Unsafe::getAndAddLong(*this, VALUE, 1); }

        glong AtomicLong::getAndDecrement() { return Unsafe::getAndAddLong(*this, VALUE, -1); }

        glong AtomicLong::getAndAdd(glong delta) { return Unsafe::getAndAddLong(*this, VALUE, delta); }

        glong AtomicLong::incrementAndGet() { return Unsafe::getAndAddLong(*this, VALUE, 1) + 1; }

        glong AtomicLong::decrementAndGet() { return Unsafe::getAndAddLong(*this, VALUE, -1) - 1; }

        glong AtomicLong::addAndGet(glong delta) { return Unsafe::getAndAddLong(*this, VALUE, delta) + delta; }

        glong AtomicLong::intValue() const { return (gint) get(); }

        glong AtomicLong::longValue() const { return get(); }

        gfloat AtomicLong::floatValue() const { return (gfloat) get(); }

        gdouble AtomicLong::doubleValue() const { return (gdouble) get(); }

        glong AtomicLong::getPlain() const { return Unsafe::getLong(*this, VALUE); }

        void AtomicLong::setPlain(glong newValue) { Unsafe::putLong(*this, VALUE, newValue); }

        glong AtomicLong::getOpaque() const { return Unsafe::getLongRelaxed(*this, VALUE); }

        void AtomicLong::setOpaque(glong newValue) { Unsafe::putLongRelaxed(*this, VALUE, newValue); }

        glong AtomicLong::getAcquire() const { return Unsafe::getLongAcquire(*this, VALUE); }

        void AtomicLong::setRelease(glong newValue) { Unsafe::putLongRelease(*this, VALUE, newValue); }

        glong AtomicLong::compareAndExchange(glong expectedValue, glong newValue) {
            return Unsafe::compareAndExchangeLong(*this, VALUE, expectedValue, newValue);
        }

        glong AtomicLong::compareAndExchangeAcquire(glong expectedValue, glong newValue) {
            return Unsafe::compareAndExchangeLongAcquire(*this, VALUE, expectedValue, newValue);
        }

        glong AtomicLong::compareAndExchangeRelease(glong expectedValue, glong newValue) {
            return Unsafe::compareAndExchangeLongRelease(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetVolatile(glong expectedValue, glong newValue) {
            return Unsafe::weakCompareAndSetLong(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetAcquire(glong expectedValue, glong newValue) {
            return Unsafe::weakCompareAndSetLongAcquire(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetRelease(glong expectedValue, glong newValue) {
            return Unsafe::weakCompareAndSetLongRelease(*this, VALUE, expectedValue, newValue);
        };
        CORE_WARNING_POP
    } // core
} // concurrent