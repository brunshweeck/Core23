//
// Created by T.N.Brunshweeck on 18/11/2023.
//

#include "AtomicBoolean.h"
#include <core/Boolean.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent {

        using namespace native;
        using namespace util;

        String AtomicBoolean::toString() const { return Boolean::toString(get()); }
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicBoolean::VALUE = CORE_OFFSETOF(AtomicBoolean, value)

        constexpr AtomicBoolean::AtomicBoolean(gbool value) : value(value ? 1 : 0) {}

        void AtomicBoolean::set(gbool newValue) { value = newValue ? 1 : 0; }

        void AtomicBoolean::lazySet(gbool newValue) { Unsafe::putIntRelease(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::getAndSet(gbool newValue) { return Unsafe::getAndSetInt(*this, VALUE, newValue ? 1 : 0) != 0; }

        gbool AtomicBoolean::compareAndSet(gbool expectedValue, gbool newValue) {
            return Unsafe::compareAndSetInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSet(gbool expectedValue, gbool newValue) {
            return Unsafe::weakCompareAndSetInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::getPlain() const { return Unsafe::getInt(*this, VALUE) != 0; }

        void AtomicBoolean::setPlain(gbool newValue) { Unsafe::putInt(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::getOpaque() const { return Unsafe::getIntRelaxed(*this, VALUE) != 0; }

        void AtomicBoolean::setOpaque(gbool newValue) { Unsafe::putIntRelaxed(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::getAcquire() const { return Unsafe::getIntAcquire(*this, VALUE) != 0; }

        void AtomicBoolean::setRelease(gbool newValue) { Unsafe::putIntRelease(*this, VALUE, newValue ? 1 : 0); }

        gbool AtomicBoolean::compareAndExchange(gbool expectedValue, gbool newValue) {
            return Unsafe::compareAndExchangeInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::compareAndExchangeAcquire(gbool expectedValue, gbool newValue) {
            return Unsafe::compareAndExchangeIntAcquire(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::compareAndExchangeRelease(gbool expectedValue, gbool newValue) {
            return Unsafe::compareAndExchangeIntRelease(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::weakCompareAndSetVolatile(gbool expectedValue, gbool newValue) {
            return Unsafe::weakCompareAndSetInt(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetAcquire(gbool expectedValue, gbool newValue) {
            return Unsafe::weakCompareAndSetIntAcquire(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetRelease(gbool expectedValue, gbool newValue) {
            return Unsafe::weakCompareAndSetIntRelease(*this, VALUE, expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::get() const { return value != 0; };
        CORE_WARNING_POP
    } // core
} // concurrent