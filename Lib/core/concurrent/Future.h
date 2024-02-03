//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#ifndef CORE23_FUTURE_H
#define CORE23_FUTURE_H

#include <core/time/Duration.h>
#include <core/IllegalStateException.h>
#include <core/InterruptedException.h>
#include <core/concurrent/ExecutionException.h>

namespace core {
    namespace concurrent {

        /**
         * A <b> Future</b> represents the result of an asynchronous
         * computation.  Methods are provided to check if the computation is
         * complete, to wait for its completion, and to retrieve the result of
         * the computation.  The result can only be retrieved using method
         * <b> get</b> when the computation has completed, blocking if
         * necessary until it is ready.  Cancellation is performed by the
         * <b> cancel</b> method.  Additional methods are provided to
         * determine if the task completed normally or was cancelled. Once a
         * computation has completed, the computation cannot be cancelled.
         * If you would like to use a <b> Future</b> for the sake
         * of cancellability but not provide a usable result, you can
         * declare types of the form <b> Future<?></b> and
         * return <b> null</b> as a result of the underlying task.
         *
         * <p><b>Sample Usage</b> (Note that the following classes are all
         * made-up.)
         *
         * <pre> <b> @code
         * struct ArchiveSearcher { String search(String target); };
         * class App {
         *   ExecutorService executor = ...;
         *   ArchiveSearcher searcher = ...;
         *   void showSearch(String target) throws InterruptedException {
         *     Callable<String> task = () -> searcher.search(target);
         *     Future<String> future = executor.submit(task);
         *     displayOtherThings(); // do other things while searching
         *     try {
         *       displayText(future.get()); // use future
         *     } catch (ExecutionException ex) { cleanup(); return; </b>
         *   }
         * } @endcode </b></pre>
         *
         * The <b style="color: orange;"> FutureTask</b> class is an implementation of <b> Future</b> that
         * implements <b> Runnable</b>, and so may be executed by an <b> Executor</b>.
         * For example, the above construction with <b> submit</b> could be replaced by:
         * <pre> <b> @code
         * FutureTask<String> future = new FutureTask<>(task);
         * executor.execute(future); @endcode </b></pre>
         *
         * <p>Memory consistency effects: Actions taken by the asynchronous computation
         * <a href=""> <i>happen-before</i></a>
         * actions following the corresponding <b> Future.get()</b> in another thread.
         *
         * @see FutureTask
         * @see Executor
         * @author Brunshweeck Tazeussong
         * @param V The result type returned by this Future's <b> get</b> method
         */
        template<class V>
        class Future : public Object {
        public:

            /**
             * Attempts to cancel execution of this task.  This method has no
             * effect if the task is already completed or cancelled, or could
             * not be cancelled for some other reason.  Otherwise, if this
             * task has not started when <b> cancel</b> is called, this task
             * should never run.  If the task has already started, then the
             * <b> mayInterruptIfRunning</b> parameter determines whether the
             * thread executing this task (when known by the implementation)
             * is interrupted in an attempt to stop the task.
             *
             * <p>The return value from this method does not necessarily
             * indicate whether the task is now cancelled; use <b style="color: orange;"> 
             * Future::isCancelled</b>.
             *
             * @param mayInterruptIfRunning <b> true</b> if the thread
             * executing this task should be interrupted (if the thread is
             * known to the implementation); otherwise, in-progress tasks are
             * allowed to complete
             * @return <b> false</b> if the task could not be cancelled,
             * typically because it has already completed; <b> true</b>
             * otherwise. If two or more threads cause a task to be cancelled,
             * then at least one of them returns <b> true</b>. Implementations
             * may provide stronger guarantees.
             */
            virtual gbool cancel(gbool mayInterruptIfRunning) = 0;

            /**
             * Returns <b> true</b> if this task was cancelled before it completed
             * normally.
             *
             * @return <b> true</b> if this task was cancelled before it completed
             */
            virtual gbool isCancelled() const = 0;

            /**
             * Returns <b> true</b> if this task completed.
             *
             * Completion may be due to normal termination, an exception, or
             * cancellation -- in all of these cases, this method will return
             * <b> true</b>.
             *
             * @return <b> true</b> if this task completed
             */
            virtual gbool isDone() const = 0;

            /**
             * Waits if necessary for the computation to complete, and then
             * retrieves its result.
             *
             * @return the computed result
             * @throws CancellationException if the computation was cancelled
             * @throws ExecutionException if the computation threw an
             * exception
             * @throws InterruptedException if the current thread was interrupted
             * while waiting
             */
            virtual V &get() = 0;

            /**
             * Waits if necessary for the computation to complete, and then
             * retrieves its result.
             *
             * @return the computed result
             * @throws CancellationException if the computation was cancelled
             * @throws ExecutionException if the computation threw an
             * exception
             * @throws InterruptedException if the current thread was interrupted
             * while waiting
             */
            virtual V const &get() const = 0;

            /**
             * Waits if necessary for at most the given time for the computation
             * to complete, and then retrieves its result, if available.
             *
             * @return the computed result
             * @throws CancellationException if the computation was cancelled
             * @throws ExecutionException if the computation threw an
             * exception
             * @throws InterruptedException if the current thread was interrupted
             * while waiting
             * @throws TimeoutException if the wait timed out
             */
            virtual V &get(time::Duration const &timeout) = 0;

            /**
             * Waits if necessary for at most the given time for the computation
             * to complete, and then retrieves its result, if available.
             *
             * @param timeout the maximum time to wait
             * @param unit the time unit of the timeout argument
             * @return the computed result
             * @throws CancellationException if the computation was cancelled
             * @throws ExecutionException if the computation threw an
             * exception
             * @throws InterruptedException if the current thread was interrupted
             * while waiting
             * @throws TimeoutException if the wait timed out
             */
            virtual V const &get(time::Duration const &timeout) const = 0;

            /**
             * Returns the computed result, without waiting.
             *
             * <p> This method is for cases where the caller knows that the task has
             * already completed successfully, for example when filtering a stream
             * of Future objects for the successful tasks and using a mapping
             * operation to obtain a stream of results.
             * {@snippet lang=java :
             *     results = futures.stream()
             *                .filter(f -> f.state() == Future.State.SUCCESS)
             *                .map(Future::resultNow)
             *                .toList();
             * </b>
             *
             * @implSpec
             * The default implementation invokes <b> isDone()</b> to test if the task
             * has completed. If done, it invokes <b> get()</b> to obtain the result.
             *
             * @return the computed result
             * @throws IllegalStateException if the task has not completed or the task
             * did not complete with a result
             */
            virtual V &resultNow() {
                if (!isDone()) {
                    IllegalStateException().throws(__ctrace());
                }
                while(true) {
                    try{
                        return get();
                    } catch(InterruptedException const &ex) {
                        Thread::currentThread().interrupt();
                        ex.throws(__ctrace());
                    } catch(ExecutionException const &ex) {
                    } catch(CancellationException const &ex) {
                    } catch (const Throwable &th) {
                        th.throws(__ctrace());
                    }
                }
            }

            /**
             * Returns the computed result, without waiting.
             *
             * <p> This method is for cases where the caller knows that the task has
             * already completed successfully, for example when filtering a stream
             * of Future objects for the successful tasks and using a mapping
             * operation to obtain a stream of results.
             * {@snippet lang=java :
             *     results = futures.stream()
             *                .filter(f -> f.state() == Future.State.SUCCESS)
             *                .map(Future::resultNow)
             *                .toList();
             * </b>
             *
             * @implSpec
             * The default implementation invokes <b> isDone()</b> to test if the task
             * has completed. If done, it invokes <b> get()</b> to obtain the result.
             *
             * @return the computed result
             * @throws IllegalStateException if the task has not completed or the task
             * did not complete with a result
             * @since 19
             */
            virtual V const &resultNow() const = 0;

            /**
             * Returns the exception thrown by the task, without waiting.
             *
             * <p> This method is for cases where the caller knows that the task
             * has already completed with an exception.
             *
             * @implSpec
             * The default implementation invokes <b> isDone()</b> to test if the task
             * has completed. If done and not cancelled, it invokes <b> get()</b> and
             * catches the <b> ExecutionException</b> to obtain the exception.
             *
             * @return the exception thrown by the task
             * @throws IllegalStateException if the task has not completed, the task
             * completed normally, or the task was cancelled
             */
            virtual Optional<Throwable> exceptionNow() const = 0;

        protected:

            /**
             * Represents the computation state.
             */
            enum State {
                /**
                 * The task has not completed.
                 */
                RUNNING,
                /**
                 * The task completed with a result.
                 * @see Future::resultNow()
                 */
                SUCCESS,
                /**
                 * The task completed with an exception.
                 * @see Future::exceptionNow()
                 */
                FAILED,
                /**
                 * The task was cancelled.
                 * @see Future::cancel(boolean)
                 */
                CANCELLED
            };

            /**
             * @return the computation state
             *
             * @implSpec
             * The default implementation uses <b> isDone()</b>, <b> isCancelled()</b>,
             * and <b> get()</b> to determine the state.
             */
            virtual State state() const = 0;
        };

    } // concurrent
} // core

#endif //CORE23_FUTURE_H
