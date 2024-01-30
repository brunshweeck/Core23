//
// Created by T.N.Brunshweeck on 26/01/2024.
//

#include "SimpleLock.h"
#include <core/IllegalStateException.h>
#include <core/CloneNotSupportedException.h>
#include <core/private/Unsafe.h>

namespace core {
    namespace concurrent {
        SimpleLock::SimpleLock() : state(UNLOCKED), owner(0), holder(0) {}

        void SimpleLock::lock() {
            if (!initialLock()) {
                tryLock0(Duration::ofMillis(Long::MAX_VALUE));
            }
        }

        gbool SimpleLock::tryLock() {
            return initialLock();
        }

        gbool SimpleLock::tryLock(const Duration &time) {
            if (!initialLock()) {
                if (!time.isPositive()) {
                    return tryLock0(time);
                }
                return false;
            }
            return true;
        }

        void SimpleLock::unlock() {
            if (!initialUnlock()) {
                IllegalStateException("Lock is already unlocked").throws(__trace("core.concurrent.SimpleLock"));
            }
        }

        gbool SimpleLock::isLocked() const {
            return state.getOpaque() == LOCKED;
        }

        Condition &SimpleLock::condition() const {
            CORE_UNREACHABLE();
        }

        SimpleLock::~SimpleLock() {
            if (isLocked()) {
                unlock();
            }
        }

        gbool SimpleLock::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<SimpleLock>::hasInstance(o))
                return false;
            SimpleLock const &lock = (SimpleLock const &) o;
            return state == lock.state && owner == lock.owner;
        }

        Object &SimpleLock::clone() const {
            if (isLocked()) {
                CloneNotSupportedException("Lock is locked").throws(__trace("core.concurrent.Lock"));
            } else {
                SimpleLock &lock = Unsafe::allocateInstance<SimpleLock>();
                lock.state.setOpaque(state.getOpaque());
                return lock;
            }
            return null;
        }

        String SimpleLock::toString() const {
            if (isLocked()) {
                return L"Lock[Locked]"_S;
            }
            return L"Lock[Unlocked]"_S;
        }

        gint SimpleLock::hash() const {
            return state.get();
        }

        gbool SimpleLock::initialLock() {
            if(state.compareAndExchangeAcquire(UNLOCKED, LOCKED) == LOCKED) {
                initialLock0();
                return true;
            }
            return false;
        }

        gbool SimpleLock::initialUnlock() {
            if(state.compareAndExchangeAcquire(LOCKED, UNLOCKED) == UNLOCKED) {
                owner.setOpaque(0L);
                return true;
            }
            return false;
        }

        SimpleLock::SimpleCondition::SimpleCondition(SimpleLock &lock) : lock(lock) {}

        void SimpleLock::SimpleCondition::wait() {
            //
        }

        glong SimpleLock::SimpleCondition::wait(glong nanosTimeout) {
            return 0;
        }

        gbool SimpleLock::SimpleCondition::wait(glong time, Temporal::ChronoUnit unit) {
            return 0;
        }

        gbool SimpleLock::SimpleCondition::wait(const Duration &deadline) {
            return 0;
        }

        void SimpleLock::SimpleCondition::signal() {
            //
        }

        void SimpleLock::SimpleCondition::signalAll() {
            //
        }

        gbool SimpleLock::SimpleCondition::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<SimpleCondition>::hasInstance(o))
                return false;
            SimpleCondition const &cnd = (SimpleCondition const &) o;
            return &cnd.lock == &lock;
        }
    } // concurrent
} // core
