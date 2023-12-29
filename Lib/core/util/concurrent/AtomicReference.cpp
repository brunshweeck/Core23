//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#include "AtomicReference.h"

namespace core {
    namespace util {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_INVALID_OFFSETOF
        gint const AtomicReference::VALUE = CORE_OFFSETOF(AtomicReference, value)

        void AtomicReference::set(Object &newValue) { value = &newValue; }

        void AtomicReference::lazySet(Object &newValue) { U::putReferenceRelease(*this, VALUE, newValue); }

        Object &AtomicReference::getAndSet(Object &newValue) { return U::getAndSetReference(*this, VALUE, newValue); }

        gbool AtomicReference::compareAndSet(const Object &expectedValue, Object &newValue) {
            return U::compareAndSetReference(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicReference::weakCompareAndSet(const Object &expectedValue, Object &newValue) {
            return U::weakCompareAndSetReference(*this, VALUE, expectedValue, newValue);
        }

        String AtomicReference::toString() const {
            return String::valueOf(get());
        }

        Object &AtomicReference::getPlain() const { return U::getReference(*this, VALUE); }

        void AtomicReference::setPlain(Object &newValue) { U::putReference(*this, VALUE, newValue); }

        Object &AtomicReference::getOpaque() const { return U::getReferenceRelaxed(*this, VALUE); }

        void AtomicReference::setOpaque(Object &newValue) { U::putReferenceRelaxed(*this, VALUE, newValue); }

        Object &AtomicReference::getAcquire() const { return U::getReferenceAcquire(*this, VALUE); }

        void AtomicReference::setRelease(Object &newValue) { U::putReferenceRelease(*this, VALUE, newValue); }

        Object &AtomicReference::compareAndExchange(const Object &expectedValue, Object &newValue) {
            return U::compareAndExchangeReference(*this, VALUE, expectedValue, newValue);
        }

        Object &AtomicReference::compareAndExchangeAcquire(const Object &expectedValue, Object &newValue) {
            return U::compareAndExchangeReferenceAcquire(*this, VALUE, expectedValue, newValue);
        }

        Object &AtomicReference::compareAndExchangeRelease(const Object &expectedValue, Object &newValue) {
            return U::compareAndExchangeReferenceRelease(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicReference::weakCompareAndSetVolatile(const Object &expectedValue, Object &newValue) {
            return U::weakCompareAndSetReference(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicReference::weakCompareAndSetAcquire(const Object &expectedValue, Object &newValue) {
            return U::weakCompareAndSetReferenceAcquire(*this, VALUE, expectedValue, newValue);
        }

        gbool AtomicReference::weakCompareAndSetRelease(const Object &expectedValue, Object &newValue) {
            return U::weakCompareAndSetReferenceRelease(*this, VALUE, expectedValue, newValue);
        };
        CORE_WARNING_POP
    } // core
} // concurrent