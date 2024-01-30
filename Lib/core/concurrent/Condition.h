//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_CONDITION_H
#define CORE23_CONDITION_H

#include <core/time/Temporal.h>
#include <core/time/Duration.h>

namespace core {
    namespace concurrent {

        using namespace time;

        /**
         * <b> Condition</b>  factors out the <b> Object</b>  monitor
         * methods into distinct objects to
         * give the effect of having multiple wait-sets per object, by
         * combining them with the use of arbitrary <b style="color: orange;"> Lock</b>  implementations.
         * Where a <b> Lock</b>  replaces the use of <b> synchronized</b>  methods
         * and statements, a <b> Condition</b>  replaces the use of the Object
         * monitor methods.
         *
         * <p>Conditions (also known as <em>condition queues</em> or
         * <em>condition variables</em>) provide a means for one thread to
         * suspend execution (to &quot;wait&quot;) until notified by another
         * thread that some state condition may now be true.  Because access
         * to this shared state information occurs in different threads, it
         * must be protected, so a lock of some form is associated with the
         * condition. The key property that waiting for a condition provides
         * is that it <em>atomically</em> releases the associated lock and
         * suspends the current thread, just like <b> Object.wait</b> .
         *
         * <p>A <b> Condition</b>  instance is intrinsically bound to a lock.
         * To obtain a <b> Condition</b>  instance for a particular <b style="color: orange;"> Lock</b> 
         * instance use its <b style="color: orange;"> Lock.condition()</b>  method.
         *
         * <p>As an example, suppose we have a bounded buffer which supports
         * <b> put</b>  and <b> take</b>  methods.  If a
         * <b> take</b>  is attempted on an empty buffer, then the thread will block
         * until an item becomes available; if a <b> put</b>  is attempted on a
         * full buffer, then the thread will block until a space becomes available.
         * We would like to keep waiting <b> put</b>  threads and <b> take</b> 
         * threads in separate wait-sets so that we can use the optimization of
         * only notifying a single thread at a time when items or spaces become
         * available in the buffer. This can be achieved using two
         * <b style="color: orange;"> Condition</b>  instances.
         * <pre>
         * @code
         * template &lt;class E&gt;
         * class BoundedBuffer {
         *   <b> Lock &lock = Unsafe::allocateInstance&lt;ReentrantLock&gt;();</b>
         *   Condition notFull  = <b>lock.condition(); </b>
         *   Condition notEmpty = <b>lock.condition(); </b>
         *
         *   Array<Object> items = Array<Object>(100);
         *   gint putptr, takeptr, count;
         *
         *   public:
         *   void put(E x) throws InterruptedException {
         *     <b>lock.lock();
         *     try {</b>
         *       while (count == items.length)
         *         <b>notFull.wait();</b>
         *       items[putptr] = x;
         *       if (++putptr == items.length) putptr = 0;
         *       ++count;
         *       <b>notEmpty.signal();</b>
         *       lock.unlock();
         *     <b>}  catch(...) {
         *       lock.unlock();
         *     } </b>
         *   }
         *
         *   E take()  {
         *     <b>lock.lock();
         *     try {</b>
         *       while (count == 0)
         *         <b>notEmpty.wait();</b>
         *       E x = (E) items[takeptr];
         *       if (++takeptr == items.length) takeptr = 0;
         *       --count;
         *       <b>notFull.signal();</b>
         *       lock.unlock();
         *       return x;
         *     <b>}  catch(...) {
         *       lock.unlock();
         *     } </b>
         *   }
         * @endcode
         * </pre>
         *
         * (The <b style="color: orange;"> ArrayBlockingQueue</b>  class provides
         * this functionality, so there is no reason to implement this
         * sample usage class.)
         *
         * <p>A <b> Condition</b>  implementation can provide behavior and semantics
         * that is
         * different from that of the <b> Object</b>  monitor methods, such as
         * guaranteed ordering for notifications, or not requiring a lock to be held
         * when performing notifications.
         * If an implementation provides such specialized semantics then the
         * implementation must document those semantics.
         *
         * <p>Note that <b> Condition</b>  instances are just normal objects and can
         * themselves be used as the target in a <b> synchronized</b>  statement,
         * and can have their own monitor <b style="color: orange;"> wait</b>  and
         * <b style="color: orange;"> notify</b>  methods invoked.
         * Acquiring the monitor lock of a <b> Condition</b>  instance, or using its
         * monitor methods, has no specified relationship with acquiring the
         * <b style="color: orange;"> Lock</b>  associated with that <b> Condition</b>  or the use of its
         * <b style="color: green;"> waiting</b>  and <b style="color: green;"> signalling</b>  methods.
         * It is recommended that to avoid confusion you never use <b> Condition</b> 
         * instances in this way, except perhaps within their own implementation.
         *
         * <p>Except where noted, passing a <b> null</b>  value for any parameter
         * will result in a <b style="color: orange;"> NullPointerException</b>  being thrown.
         *
         * <h2>Implementation Considerations</h2>
         *
         * <p>When waiting upon a <b> Condition</b> , a &quot;<em>spurious
         * wakeup</em>&quot; is permitted to occur, in
         * general, as a concession to the underlying platform semantics.
         * This has little practical impact on most application programs as a
         * <b> Condition</b>  should always be waited upon in a loop, testing
         * the state predicate that is being waited for.  An implementation is
         * free to remove the possibility of spurious wakeups but it is
         * recommended that applications programmers always assume that they can
         * occur and so always wait in a loop.
         *
         * <p>The three forms of condition waiting
         * (interruptible, non-interruptible, and timed) may differ in their ease of
         * implementation on some platforms and in their performance characteristics.
         * In particular, it may be difficult to provide these features and maintain
         * specific semantics such as ordering guarantees.
         * Further, the ability to interrupt the actual suspension of the thread may
         * not always be feasible to implement on all platforms.
         *
         * <p>Consequently, an implementation is not required to define exactly the
         * same guarantees or semantics for all three forms of waiting, nor is it
         * required to support interruption of the actual suspension of the thread.
         *
         * <p>An implementation is required to
         * clearly document the semantics and guarantees provided by each of the
         * waiting methods, and when an implementation does support interruption of
         * thread suspension then it must obey the interruption semantics as defined
         * in this interface.
         *
         * <p>As interruption generally implies cancellation, and checks for
         * interruption are often infrequent, an implementation can favor responding
         * to an interrupt over normal method return. This is true even if it can be
         * shown that the interrupt occurred after another action that may have
         * unblocked the thread. An implementation should document this behavior.
         *
         * @author Brunshweeck Tazeussong
         */
        class Condition: public Object {
        public:

            /**
             * Causes the current thread to wait until it is signalled or
             * <b style="color: green;"> interrupted</b> .
             *
             * <p>The lock associated with this <b> Condition</b>  is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of four things happens:
             * <ul>
             * <li>Some other thread invokes the <b style="color: orange;"> Condition.signal</b>  method for this
             * <b> Condition</b>  and the current thread happens to be chosen as the
             * thread to be awakened; or
             * <li>Some other thread invokes the <b style="color: orange;"> Condition.signalAll</b>  method for this
             * <b> Condition</b> ; or
             * <li>Some other thread <b style="color: green;"> interrupts</b>  the
             * current thread, and interruption of thread suspension is supported; or
             * <li>A &quot;<em>spurious wakeup</em>&quot; occurs.
             * </ul>
             *
             * <p>In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             *
             * <p>If the current thread:
             * <ul>
             * <li>has its interrupted status setValue on entry to this method; or
             * <li>is <b style="color: green;"> interrupted</b>  while waiting
             * and interruption of thread suspension is supported,
             * </ul>
             * then <b style="color: orange;"> InterruptedException</b>  is thrown and the current thread's
             * interrupted status is cleared. It is not specified, in the first
             * case, whether or not the test for interruption occurs before the lock
             * is released.
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>The current thread is assumed to hold the lock associated with this
             * <b> Condition</b>  when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as <b style="color: orange;"> IllegalMonitorStateException</b> ) and the
             * implementation must document that fact.
             *
             * <p>An implementation can favor responding to an interrupt over normal
             * method return in response to a signal. In that case the implementation
             * must ensure that the signal is redirected to another waiting thread, if
             * there is one.
             *
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
             virtual void wait() = 0;

            /**
             * Causes the current thread to wait until it is signalled or interrupted,
             * or the specified waiting time elapses.
             *
             * <p>The lock associated with this condition is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of five things happens:
             * <ul>
             * <li>Some other thread invokes the <b style="color: orange;"> Condition.signal</b>  method for this
             * <b> Condition</b>  and the current thread happens to be chosen as the
             * thread to be awakened; or
             * <li>Some other thread invokes the <b style="color: orange;"> Condition.signalAll</b>  method for this
             * <b> Condition</b> ; or
             * <li>Some other thread <b style="color: green;"> interrupts</b>  the
             * current thread, and interruption of thread suspension is supported; or
             * <li>The specified waiting time elapses; or
             * <li>A &quot;<em>spurious wakeup</em>&quot; occurs.
             * </ul>
             *
             * <p>In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             *
             * <p>If the current thread:
             * <ul>
             * <li>has its interrupted status setValue on entry to this method; or
             * <li>is <b style="color: green;"> interrupted</b>  while waiting
             * and interruption of thread suspension is supported,
             * </ul>
             * then <b style="color: orange;"> InterruptedException</b>  is thrown and the current thread's
             * interrupted status is cleared. It is not specified, in the first
             * case, whether or not the test for interruption occurs before the lock
             * is released.
             *
             * <p>The method returns an estimate of the number of nanoseconds
             * remaining to wait given the supplied <b> nanosTimeout</b> 
             * value upon return, or a value less than or equal to zero if it
             * timed out. This value can be used to determine whether and how
             * glong to re-wait in cases where the wait returns but an waited
             * condition still does not hold. Typical uses of this method take
             * the following form:
             *
             * <pre> <b>
             * @code
             * gbool aMethod(glong timeout, ChronoUnit unit) {
             *   glong nanosRemaining = unit.toNanos(timeout);
             *   lock.lock();
             *   try {
             *     while (!conditionBeingWaitedFor()) {
             *       if (nanosRemaining <= 0L) {
             *         lock.unlock();
             *         return false;
             *       }
             *       nanosRemaining = theCondition.wait(nanosRemaining);
             *     } 
             *     // ...
             *     lock.unlock();
             *     return true;
             *   }  finally {
             *     lock.unlock();
             *   } 
             * }
             * @endcode </b> </pre>
             *
             * <p>Design note: This method requires a nanosecond argument so
             * as to avoid truncation errors in reporting remaining times.
             * Such precision loss would make it difficult for programmers to
             * ensure that total waiting times are not systematically shorter
             * than specified when re-waits occur.
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>The current thread is assumed to hold the lock associated with this
             * <b> Condition</b>  when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as <b style="color: orange;"> IllegalMonitorStateException</b> ) and the
             * implementation must document that fact.
             *
             * <p>An implementation can favor responding to an interrupt over normal
             * method return in response to a signal, or over indicating the elapse
             * of the specified waiting time. In either case the implementation
             * must ensure that the signal is redirected to another waiting thread, if
             * there is one.
             *
             * @param nanosTimeout the maximum time to wait, in nanoseconds
             * @return an estimate of the <b> nanosTimeout</b>  value minus
             *         the time spent waiting upon return from this method.
             *         A positive value may be used as the argument to a
             *         subsequent call to this method to finish waiting out
             *         the desired time.  A value less than or equal to zero
             *         indicates that no time remains.
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
             virtual glong wait(glong nanosTimeout) = 0;

            /**
             * Causes the current thread to wait until it is signalled or interrupted,
             * or the specified waiting time elapses. This method is behaviorally
             * equivalent to:
             * <pre> <b> @code wait(unit.toNanos(time)) > 0 @endcode </b> </pre>
             *
             * @param time the maximum time to wait
             * @param unit the time unit of the <b> time</b>  argument
             * @return <b> false</b>  if the waiting time detectably elapsed
             *         before return from the method, else <b> true</b> 
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
             virtual gbool wait(glong time, Temporal::ChronoUnit unit) = 0;

            /**
             * Causes the current thread to wait until it is signalled or interrupted,
             * or the specified deadline elapses.
             *
             * <p>The lock associated with this condition is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of five things happens:
             * <ul>
             * <li>Some other thread invokes the <b style="color: orange;"> Condition.signal</b>  method for this
             * <b> Condition</b>  and the current thread happens to be chosen as the
             * thread to be awakened; or
             * <li>Some other thread invokes the <b style="color: orange;"> Condition.signalAll</b>  method for this
             * <b> Condition</b> ; or
             * <li>Some other thread <b style="color: green;"> interrupts</b>  the
             * current thread, and interruption of thread suspension is supported; or
             * <li>The specified deadline elapses; or
             * <li>A &quot;<em>spurious wakeup</em>&quot; occurs.
             * </ul>
             *
             * <p>In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             *
             * <p>If the current thread:
             * <ul>
             * <li>has its interrupted status setValue on entry to this method; or
             * <li>is <b style="color: green;"> interrupted</b>  while waiting
             * and interruption of thread suspension is supported,
             * </ul>
             * then <b style="color: orange;"> InterruptedException</b>  is thrown and the current thread's
             * interrupted status is cleared. It is not specified, in the first
             * case, whether or not the test for interruption occurs before the lock
             * is released.
             *
             * <p>The return value indicates whether the deadline has elapsed,
             * which can be used as follows:
             * <pre> <b>
             * @code
             * gbool aMethod(Duration deadline) {
             *   gbool stillWaiting = true;
             *   lock.lock();
             *   try {
             *     while (!conditionBeingWaitedFor()) {
             *       if (!stillWaiting){
             *         lock.unlock();
             *         return false;
             *       }
             *       stillWaiting = theCondition.wait(deadline);
             *     }
             *     // ...
             *     lock.unlock();
             *     return true;
             *   }  catch (...) {
             *     lock.unlock();
             *   }
             * }
             * @endcode </b> </pre>
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>The current thread is assumed to hold the lock associated with this
             * <b> Condition</b>  when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as <b style="color: orange;"> IllegalMonitorStateException</b> ) and the
             * implementation must document that fact.
             *
             * <p>An implementation can favor responding to an interrupt over normal
             * method return in response to a signal, or over indicating the passing
             * of the specified deadline. In either case the implementation
             * must ensure that the signal is redirected to another waiting thread, if
             * there is one.
             *
             * @param deadline the absolute time to wait until
             * @return <b> false</b>  if the deadline has elapsed upon return, else
             *         <b> true</b> 
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
             virtual gbool wait(const Duration& deadline) = 0;

            /**
             * Wakes up one waiting thread.
             *
             * <p>If any threads are waiting on this condition then one
             * is selected for waking up. That thread must then re-acquire the
             * lock before returning from <b> wait</b> .
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>An implementation may (and typically does) require that the
             * current thread hold the lock associated with this <b> 
             * Condition</b>  when this method is called. Implementations must
             * document this precondition and any actions taken if the lock is
             * not held. Typically, an exception such as <b style="color: orange;"> 
             * IllegalMonitorStateException</b>  will be thrown.
             */
             virtual void signal() = 0;

            /**
             * Wakes up all waiting threads.
             *
             * <p>If any threads are waiting on this condition then they are
             * all woken up. Each thread must re-acquire the lock before it can
             * return from <b> wait</b> .
             *
             * <p><b>Implementation Considerations</b>
             *
             * <p>An implementation may (and typically does) require that the
             * current thread hold the lock associated with this <b> 
             * Condition</b>  when this method is called. Implementations must
             * document this precondition and any actions taken if the lock is
             * not held. Typically, an exception such as <b style="color: orange;"> 
             * IllegalMonitorStateException</b>  will be thrown.
             */
             virtual void signalAll() = 0;
        };

    } // core
} // concurrent

#endif //CORE23_CONDITION_H
