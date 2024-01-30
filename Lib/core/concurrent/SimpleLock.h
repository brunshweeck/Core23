//
// Created by T.N.Brunshweeck on 26/01/2024.
//

#ifndef MYHARDLINK_SIMPLELOCK_H
#define MYHARDLINK_SIMPLELOCK_H

#include <core/concurrent/AtomicInteger.h>
#include <core/concurrent/AtomicLong.h>
#include <core/concurrent/AtomicBoolean.h>
#include <core/concurrent/AtomicLongArray.h>
#include <core/concurrent/Lock.h>
#include <core/concurrent/Condition.h>

namespace core {
    namespace concurrent {

        /**
         * SimpleLock represent the basic implementation of <b> Lock</b>.
         *
         * <p>A lock is a tool for controlling access to a shared resource by
         * multiple threads. Commonly, a lock provides exclusive access to a
         * shared resource: only one thread at a time can acquire the lock and
         * all access to the shared resource requires that the lock be
         * acquired first.
         *
         */
        class SimpleLock CORE_FINAL : public Lock {
        private:

            /**
             * The flag representing the
             * state of this lock
             */
            enum SyncState : gbyte {

                /**
                 * The lock flag.
                 * Used to mark that shared ressource is now accessible
                 * for any thread
                 */
                UNLOCKED = 1,

                /**
                 * The unlock flag.
                 * Used to mark that shared ressource is now accessible
                 * for owner thread only
                 */
                LOCKED = 2,
            };

            class SimpleCondition CORE_FINAL : public Condition {
            private:
                SimpleLock &lock;

            public:
                /**
                 * Create new instance of this Condition
                 */
                CORE_EXPLICIT SimpleCondition(SimpleLock &lock);

                void wait() override;

                glong wait(glong nanosTimeout) override;

                gbool wait(glong time, Temporal::ChronoUnit unit) override;

                gbool wait(const Duration &deadline) override;

                void signal() override;

                void signalAll() override;

                gbool equals(const Object &o) const override;
            };

            /**
             * The state of lock
             */
            AtomicInteger state;

            /**
             * The owner ID of lock
             */
            AtomicLong owner;

            /**
             * The lock events/attribute
             */
            AtomicLongArray holder;

        public:
            /**
             * Create new instance of Simple lock
             */
            SimpleLock();

            /**
             * Acquire lock for current thread
             */
            void lock() override;

            /**
             * Acquires the lock for current thread if only if
             * the shared ressource is accessible at the time of invocation.
             *
             * @return <b> true</b> if the lock was acquired and
             *         <b> false</b> otherwise
             */
            gbool tryLock() override;

            /**
             * Acquires the lock for current thread if only if
             * the shared ressource is accessible after the given waiting time.
             *
             * @param time time the maximum time to wait for the lock
             * @return <b> true</b> if the lock was acquired and
             *         <b> false</b> otherwise
             */
            gbool tryLock(const Duration &time) override;

            /**
             * Release the lock
             *
             * @throws IllegalStateException If this lock is already unlocked
             */
            void unlock() override;

            /**
             * Tell if the lock is acquired or released
             */
            gbool isLocked() const override;

            /**
             * Return new instance of condition
             * associated with this lock.
             */
            Condition &condition() const override;

            /**
             * Release and destroy this lock
             */
            ~SimpleLock() override;

            gbool equals(const Object &o) const override;

            Object &clone() const override;

            String toString() const override;

            gint hash() const override;

        private:
            /**
             * Acquire lock for first time
             */
            gbool initialLock();

            void initialLock0();

            /**
             * Release first time.
             */
            gbool initialUnlock();

            /**
             * Try acquire lock during specified time (internal)
             */
            gbool tryLock0(const Duration &duration);

        };

    } // concurrent
} // core

#endif //MYHARDLINK_SIMPLELOCK_H
