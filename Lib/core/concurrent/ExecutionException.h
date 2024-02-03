//
// Created by T.N.Brunshweeck on 02/02/2024.
//

#ifndef CORE23_EXECUTIONEXCEPTION_H
#define CORE23_EXECUTIONEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace concurrent {

        /**
         * Exception thrown when attempting to retrieve the result of a task
         * that aborted by throwing an exception. This exception can be
         * inspected using the <b style="color: orange;"> #getCause()</b> method.
         *
         * @see Future
         * @author Brunshweeck Tazeussong
         */
        class ExecutionException: public Exception {
        public:

            /**
             * Constructs an <b> ExecutionException</b> with no detail message.
             * The cause is not initialized, and may subsequently be
             * initialized by a call to <b style="color: orange;"> #initCause(Throwable) initCause</b>.
             */
            ExecutionException() CORE_NOTHROW = default;

            /**
             * Constructs an <b> ExecutionException</b> with the specified detail
             * message. The cause is not initialized, and may subsequently be
             * initialized by a call to <b style="color: orange;"> #initCause(Throwable) initCause</b>.
             *
             * @param message the detail message
             */
            CORE_EXPLICIT ExecutionException(String message) CORE_NOTHROW;

            /**
             * Constructs an <b> ExecutionException</b> with the specified detail
             * message and cause.
             *
             * @param  message the detail message
             * @param  cause the cause (which is saved for later retrieval by the
             *         <b style="color: orange;"> cause()</b> method)
             */
            CORE_EXPLICIT ExecutionException(String message, const Throwable &cause) CORE_NOTHROW;

            /**
             * Constructs an <b> ExecutionException</b> with the specified cause.
             * The detail message is set to <b> cause.toString()</b> (which typically contains the class and
             * detail message of <b> cause</b>).
             *
             * @param  cause the cause (which is saved for later retrieval by the
             *         <b style="color: orange;"> cause()</b> method)
             */
            CORE_EXPLICIT ExecutionException(const Throwable &cause);

            Object &clone() const override;

        private:
            void raise() && override;
        };

    } // concurrent
} // core

#endif //CORE23_EXECUTIONEXCEPTION_H
