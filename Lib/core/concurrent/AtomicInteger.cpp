//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicInteger.h"
#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent  {

        using namespace native;
        using namespace util;

CORE_WARNING_PUSH
CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicInteger::VALUE = CORE_OFFSETOF(AtomicInteger, value)

        void AtomicInteger::lazySet(gint newValue) { Unsafe::putIntRelease(*this, VALUE, newValue); }

        gint AtomicInteger::getAndSet(gint newValue) { return Unsafe::getAndSetInt(*this, VALUE, newValue); }

        gbool AtomicInteger::compareAndSet(gint expectedValue, gint newValue) {
            return Unsafe::compareAndSetInt(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSet(gint expectedValue, gint newValue) {
            return Unsafe::weakCompareAndSetInt(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::getAndIncrement() { return Unsafe::getAndAddInt(*this, VALUE, 1); }

        gint AtomicInteger::getAndDecrement() { return Unsafe::getAndAddInt(*this, VALUE, -1); }

        gint AtomicInteger::getAndAdd(gint delta) { return Unsafe::getAndAddInt(*this, VALUE, delta); }

        gint AtomicInteger::incrementAndGet() { return Unsafe::getAndAddInt(*this, VALUE, 1) + 1; }

        gint AtomicInteger::decrementAndGet() { return Unsafe::getAndAddInt(*this, VALUE, -1) - 1; }

        gint AtomicInteger::addAndGet(gint delta) { return Unsafe::getAndAddInt(*this, VALUE, delta) + delta; }

        String AtomicInteger::toString() const { return Integer::toString(get()); }

        gint AtomicInteger::intValue() const { return get(); }

        glong AtomicInteger::longValue() const { return (glong) get(); }

        gfloat AtomicInteger::floatValue() const { return (gfloat) get(); }

        gdouble AtomicInteger::doubleValue() const { return (gdouble) get(); }

        gint AtomicInteger::getPlain() const { return Unsafe::getInt(*this, VALUE); }

        void AtomicInteger::setPlain(gint newValue) { Unsafe::putInt(*this, VALUE, newValue); }

        gint AtomicInteger::getOpaque() const { return Unsafe::getIntRelaxed(*this, VALUE); }

        void AtomicInteger::setOpaque(gint newValue) { Unsafe::putIntRelaxed(*this, VALUE, newValue); }

        gint AtomicInteger::getAcquire() const { return Unsafe::getIntAcquire(*this, VALUE); }

        void AtomicInteger::setRelease(gint newValue) { Unsafe::putIntRelease(*this, VALUE, newValue); }

        gint AtomicInteger::compareAndExchange(gint expectedValue, gint newValue) {
            return Unsafe::compareAndExchangeInt(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::compareAndExchangeAcquire(gint expectedValue, gint newValue) {
            return Unsafe::compareAndExchangeIntAcquire(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::compareAndExchangeRelease(gint expectedValue, gint newValue) {
            return Unsafe::compareAndExchangeIntRelease(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetVolatile(gint expectedValue, gint newValue) {
            return Unsafe::weakCompareAndSetInt(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetAcquire(gint expectedValue, gint newValue) {
            return Unsafe::weakCompareAndSetIntAcquire(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetRelease(gint expectedValue, gint newValue) {
            return Unsafe::weakCompareAndSetIntRelease(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::get() const { return value; }

        void AtomicInteger::set(gint newValue) { value = newValue; };
CORE_WARNING_POP
    } // core
} // concurrent