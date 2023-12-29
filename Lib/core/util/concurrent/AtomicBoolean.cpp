//
// Created by T.N.Brunshweeck on 18/11/2023.
//

#include "AtomicBoolean.h"
#include <core/Boolean.h>

namespace core {
    namespace util {
        String AtomicBoolean::toString() const { return Boolean::toString(get()); }
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicBoolean::VALUE = CORE_OFFSETOF(AtomicBoolean, value)

        constexpr AtomicBoolean::AtomicBoolean(gbool value) : value(value ? 1 : 0) {}

        void AtomicBoolean::set(gbool newValue) { value = newValue ? 1 : 0; }

        void AtomicBoolean::lazySet(gbool newValue) { U::putIntRelease(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::getAndSet(gbool newValue) { return U::getAndSetInt(*this, VALUE, newValue ? 1 : 0) != 0; }

        gbool AtomicBoolean::compareAndSet(gbool expectedValue, gbool newValue) {
            return U::compareAndSetInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSet(gbool expectedValue, gbool newValue) {
            return U::weakCompareAndSetInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::getPlain() const { return U::getInt(*this, VALUE) != 0; }

        void AtomicBoolean::setPlain(gbool newValue) { U::putInt(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::getOpaque() const { return U::getIntRelaxed(*this, VALUE) != 0; }

        void AtomicBoolean::setOpaque(gbool newValue) { U::putIntRelaxed(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::getAcquire() const { return U::getIntAcquire(*this, VALUE) != 0; }

        void AtomicBoolean::setRelease(gbool newValue) { U::putIntRelease(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::compareAndExchange(gbool expectedValue, gbool newValue) {
            return U::compareAndExchangeInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::compareAndExchangeAcquire(gbool expectedValue, gbool newValue) {
            return U::compareAndExchangeIntAcquire(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::compareAndExchangeRelease(gbool expectedValue, gbool newValue) {
            return U::compareAndExchangeIntRelease(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::weakCompareAndSetVolatile(gbool expectedValue, gbool newValue) {
            return U::weakCompareAndSetInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetAcquire(gbool expectedValue, gbool newValue) {
            return U::weakCompareAndSetIntAcquire(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetRelease(gbool expectedValue, gbool newValue) {
            return U::weakCompareAndSetIntRelease(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        };
        CORE_WARNING_POP
    } // core
} // concurrent