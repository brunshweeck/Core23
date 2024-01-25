//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_LOCK_H
#define CORE23_LOCK_H

#include <core/concurrent/Condition.h>
#include <core/time/Duration.h>

namespace core {
    namespace concurrent {
        using namespace time;

        /**
         * <b> Lock</b> implementations provide more extensive locking
         * operations than can be obtained using <b> synchronized</b> methods
         * and statements.  They allow more flexible structuring, may have
         * quite different properties, and may support multiple associated
         * <b style="color: orange;"> Condition</b> objects.
         *
         * <p>A lock is a tool for controlling access to a shared resource by
         * multiple threads. Commonly, a lock provides exclusive access to a
         * shared resource: only one thread at a time can acquire the lock and
         * all access to the shared resource requires that the lock be
         * acquired first. However, some locks may allow concurrent access to
         * a shared resource, such as the read lock of a <b style="color: orange;"> ReadWriteLock</b>.
         *
         * <p>Implementations of the <b> Lock</b> interface enable the use of such techniques by
         * allowing a lock to be acquired and released in different scopes,
         * and allowing multiple locks to be acquired and released in any
         * order.
         *
         * <p>With this increased flexibility comes additional
         * responsibility. The absence of block-structured locking removes the
         * automatic release of locks that occurs with <b> synchronized</b>
         * methods and statements. In most cases, the following idiom
         * should be used:
         *
         * @code
         * <pre> <b>
         * Lock& l = ...;
         * l.lock();
         * try {
         *   // access the resource protected by this lock
         *   l.unlock();
         * } catch(...) {
         *   l.unlock();
         * }</b></pre>
         * @endcode
         *
         * When locking and unlocking occur in different scopes, care must be
         * taken to ensure that all code that is executed while the lock is
         * held is protected by try-catch to ensure that the
         * lock is released when necessary.
         *
         * <p><b> Lock</b> implementations provide additional functionality
         * over the use of <b> synchronized</b> methods and statements by
         * providing a non-blocking attempt to acquire a lock (<b style="color: orange;">
         * tryLock()</b>) and an attempt to acquire the lock that can timeout (<b style="color: orange;">
         * tryLock(glong, Unit)</b>).
         *
         * <p>A <b> Lock</b> class can also provide behavior and semantics
         * that is quite different from that of the implicit monitor lock,
         * such as guaranteed ordering, non-reentrant usage, or deadlock
         * detection. If an implementation provides such specialized semantics
         * then the implementation must document those semantics.
         *
         * <p>Note that <b> Lock</b> instances are just normal objects and can
         * themselves be used as the target in a <b> synchronized</b> statement.
         * Acquiring the monitor lock of a <b> Lock</b> instance has no specified relationship
         * with invoking any of the <b style="color: orange;"> lock</b> methods of that instance.
         * It is recommended that to avoid confusion you never use <b> Lock</b>
         * instances in this way, except within their own implementation.
         *
         * <p>Except where noted, passing a <b> null</b> value for any
         * parameter will result in a <b style="color: orange;"> NullPointerException</b> being
         * thrown.
         *
         * <h2>Memory Synchronization</h2>
         *
         * <p>All <b> Lock</b> implementations <em>must</em> enforce the same
         * memory synchronization semantics as provided by the built-in monitor lock
         * <ul>
         * <li>A successful <b> lock</b> operation has the same memory
         * synchronization effects as a successful <em>Lock</em> action.
         * <li>A successful <b> unlock</b> operation has the same
         * memory synchronization effects as a successful <em>Unlock</em> action.
         * </ul>
         *
         * Unsuccessful locking and unlocking operations, and reentrant
         * locking/unlocking operations, do not require any memory
         * synchronization effects.
         *
         * <h2>Implementation Considerations</h2>
         *
         * <p>The three forms of lock acquisition (interruptible,
         * non-interruptible, and timed) may differ in their performance
         * characteristics, ordering guarantees, or other implementation
         * qualities.  Further, the ability to interrupt the <em>ongoing</em>
         * acquisition of a lock may not be available in a given <b> Lock</b>
         * class.  Consequently, an implementation is not required to define
         * exactly the same guarantees or semantics for all three forms of
         * lock acquisition, nor is it required to support interruption of an
         * ongoing lock acquisition.  An implementation is required to clearly
         * document the semantics and guarantees provided by each of the
         * locking methods. It must also obey the interruption semantics as
         * defined in this interface, to the extent that interruption of lock
         * acquisition is supported: which is either totally, or only on
         * method entry.
         *
         * <p>As interruption generally implies cancellation, and checks for
         * interruption are often infrequent, an implementation can favor responding
         * to an interrupt over normal method return. This is true even if it can be
         * shown that the interrupt occurred after another action may have unblocked
         * the thread. An implementation should document this behavior.
         *
         * @see ReentrantLock
         * @see ReadWriteLock
         * @see Condition
         *
         * @author Brunshweeck Tazeussong
         */
        class Lock : public Object {
        public:
            /**
             * Acquires the lock.
             *
             * <p>
             * If the lock is not available then the current thread becomes
             * disabled for thread scheduling purposes and lies dormant until the
             * lock has been acquired.
             *
             * <p>
             * <b>Implementation Considerations</b>
             *
             * <p>A <b> Lock</b> implementation may be able to detect erroneous use
             * of the lock, such as an invocation that would cause deadlock, and
             * may throw an (unchecked) exception in such circumstances.  The
             * circumstances and the exception type must be documented by that
             * <b> Lock</b> implementation.
             */
            virtual void lock() = 0;

            /**
             * Acquires the lock only if it is free at the time of invocation.
             *
             * <p>Acquires the lock if it is available and returns immediately
             * with the value <b> true</b>.
             * If the lock is not available then this method will return
             * immediately with the value <b> false</b>.
             *
             * <p>A typical usage idiom for this method would be:
             * @code
             * <pre> <b>
             * Lock lock = ...;
             * if (lock.tryLock()) {
             *   try {
             *     // manipulate protected state
             *   } catch(...) {
             *    lock.unlock();
             *    ...
             *   }
             *   lock.unlock();
             * } else {
             *   // perform alternative actions
             * } </b></pre>
             * @endcode
             *
             * This usage ensures that the lock is unlocked if it was acquired, and
             * doesn't try to unlock if the lock was not acquired.
             *
             * @return <b> true</b> if the lock was acquired and
             *         <b> false</b> otherwise
             */
            virtual gbool tryLock() = 0;

            /**
             * Acquires the lock if it is free within the given waiting time and the
             * current thread has not been <b style="color: green;">interrupted</b>.
             *
             * <p>If the lock is available this method returns immediately
             * with the value <b> true</b>.
             * If the lock is not available then
             * the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until one of three things happens:
             * <ul>
             * <li>The lock is acquired by the current thread; or
             * <li>Some other thread <b style="color: green;">interrupts</b> the
             * current thread, and interruption of lock acquisition is supported; or
             * <li>The specified waiting time elapses
             * </ul>
             *
             * <p>If the lock is acquired then the value <b> true</b> is returned.
             *
             * <p>If the current thread:
             * <ul>
             * <li>has its interrupted status set on entry to this method; or
             * <li>is <b style="color: green;">interrupted</b> while acquiring
             * the lock, and interruption of lock acquisition is supported,
             * </ul>
             * then <b style="color: orange;"> InterruptedException</b> is thrown and the current thread's
             * interrupted status is cleared.
             *
             * <p>If the specified waiting time elapses then the value <b> false</b>
             * is returned.
             * If the time is
             * less than or equal to zero, the method will not wait at all.
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>The ability to interrupt a lock acquisition in some implementations
             * may not be possible, and if possible may
             * be an expensive operation.
             * The programmer should be aware that this may be the case. An
             * implementation should document when this is the case.
             *
             * <p>An implementation can favor responding to an interrupt over normal
             * method return, or reporting a timeout.
             *
             * <p>A <b> Lock</b> implementation may be able to detect
             * erroneous use of the lock, such as an invocation that would cause
             * deadlock, and may throw an (unchecked) exception in such circumstances.
             * The circumstances and the exception type must be documented by that
             * <b> Lock</b> implementation.
             *
             * @param time the maximum time to wait for the lock
             * @param unit the time unit of the <b> time</b> argument
             * @return <b> true</b> if the lock was acquired and <b> false</b>
             *         if the waiting time elapsed before the lock was acquired
             *
             * @throws InterruptedException if the current thread is interrupted
             *         while acquiring the lock (and interruption of lock
             *         acquisition is supported)
             */
            virtual gbool tryLock(const time::Duration &time) = 0;

            /**
             * Releases the lock.
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>A <b> Lock</b> implementation will usually impose
             * restrictions on which thread can release a lock (typically only the
             * holder of the lock can release it) and may throw
             * an (unchecked) exception if the restriction is violated.
             * Any restrictions and the exception
             * type must be documented by that <b> Lock</b> implementation.
             */
            virtual void unlock() = 0;

            /**
             * Queries if this locker is held by any thread.
             */
            virtual gbool isLocked() const = 0;

            /**
             * Returns a new <b style="color: orange;"> Condition</b> instance that is bound to this
             * <b> Lock </b> instance.
             *
             * <p>Before waiting on the condition the lock must be held by the
             * current thread.
             * A call to <b style="color: orange;"> Condition::wait()</b> will atomically release the lock
             * before waiting and re-acquire the lock before the wait returns.
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>The exact operation of the <b style="color: orange;"> Condition</b> instance depends on
             * the <b> Lock</b> implementation and must be documented by that
             * implementation.
             *
             * @throws UnsupportedOperationException if this <b> Lock</b> implementation does not support conditions
             */
            virtual Condition &condition() const = 0;
        };

    } // core
} // concurrent

#endif //CORE23_LOCK_H
