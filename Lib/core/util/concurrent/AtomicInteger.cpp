//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicInteger.h"

namespace core {
    namespace util  {
CORE_WARNING_PUSH
CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicInteger::VALUE = CORE_OFFSETOF(AtomicInteger, value)

        void AtomicInteger::lazySet(gint newValue) { U::putIntRelease(*this, VALUE, newValue); }

        gint AtomicInteger::getAndSet(gint newValue) { return U::getAndSetInt(*this, VALUE, newValue); }

        gbool AtomicInteger::compareAndSet(gint expectedValue, gint newValue) {
            return U::compareAndSetInt(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSet(gint expectedValue, gint newValue) {
            return U::weakCompareAndSetInt(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::getAndIncrement() { return U::getAndAddInt(*this, VALUE, 1); }

        gint AtomicInteger::getAndDecrement() { return U::getAndAddInt(*this, VALUE, -1); }

        gint AtomicInteger::getAndAdd(gint delta) { return U::getAndAddInt(*this, VALUE, delta); }

        gint AtomicInteger::incrementAndGet() { return U::getAndAddInt(*this, VALUE, 1) + 1; }

        gint AtomicInteger::decrementAndGet() { return U::getAndAddInt(*this, VALUE, -1) - 1; }

        gint AtomicInteger::addAndGet(int delta) { return U::getAndAddInt(*this, VALUE, delta) + delta; }

        String AtomicInteger::toString() const { return Integer::toString(get()); }

        gint AtomicInteger::intValue() const { return get(); }

        glong AtomicInteger::longValue() const { return (glong) get(); }

        gfloat AtomicInteger::floatValue() const { return (gfloat) get(); }

        gdouble AtomicInteger::doubleValue() const { return (gdouble) get(); }

        gint AtomicInteger::getPlain() const { return U::getInt(*this, VALUE); }

        void AtomicInteger::setPlain(gint newValue) { U::putInt(*this, VALUE, newValue); }

        gint AtomicInteger::getOpaque() const { return U::getIntRelaxed(*this, VALUE); }

        void AtomicInteger::setOpaque(gint newValue) { U::putIntRelaxed(*this, VALUE, newValue); }

        gint AtomicInteger::getAcquire() const { return U::getIntAcquire(*this, VALUE); }

        void AtomicInteger::setRelease(gint newValue) { U::putIntRelease(*this, VALUE, newValue); }

        gint AtomicInteger::compareAndExchange(gint expectedValue, gint newValue) {
            return U::compareAndExchangeInt(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::compareAndExchangeAcquire(gint expectedValue, gint newValue) {
            return U::compareAndExchangeIntAcquire(*this, VALUE, expectedValue, newValue);
        }

        gint AtomicInteger::compareAndExchangeRelease(gint expectedValue, gint newValue) {
            return U::compareAndExchangeIntRelease(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetVolatile(gint expectedValue, gint newValue) {
            return U::weakCompareAndSetInt(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetAcquire(gint expectedValue, gint newValue) {
            return U::weakCompareAndSetIntAcquire(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetRelease(gint expectedValue, gint newValue) {
            return U::weakCompareAndSetIntRelease(*this, VALUE, expectedValue, newValue);
        };
CORE_WARNING_POP
    } // core
} // concurrent