//
// Created by T.N.Brunshweeck on 17/11/2023.
//

#ifndef CORE23_IOEXCEPTION_H
#define CORE23_IOEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace io {

        /**
         * Signals that an I/O exception of some sort has occurred. This
         * class is the general class of exceptions produced by failed or
         * interrupted I/O operations.
         *
         */
        class IOException : public Exception {
        public:
            /**
             * Constructs an {@code IOException} with {@code null}
             * as its error detail message.
             */
            IOException() = default;

            /**
             * Constructs an {@code IOException} with the specified detail message.
             *
             * @param message
             *        The detail message (which is saved for later retrieval
             *        by the {@link #getMessage()} method)
             */
            CORE_EXPLICIT IOException(String message);

            /**
             * Constructs an {@code IOException} with the specified detail message
             * and cause.
             *
             * <p> Note that the detail message associated with {@code cause} is
             * <i>not</i> automatically incorporated into this exception's detail
             * message.
             *
             * @param message
             *        The detail message (which is saved for later retrieval
             *        by the {@link #getMessage()} method)
             *
             * @param cause
             *        The cause (which is saved for later retrieval by the
             *        {@link #getCause()} method).  (A null value is permitted,
             *        and indicates that the cause is nonexistent or unknown.)
             */
            CORE_EXPLICIT IOException(String message, const Throwable &cause);

            /**
             * Constructs an {@code IOException} with the specified cause and a
             * detail message of {@code (cause==null ? null : cause.toString())}
             * (which typically contains the class and detail message of {@code cause}).
             * This constructor is useful for IO exceptions that are little more
             * than wrappers for other throwables.
             *
             * @param cause
             *        The cause (which is saved for later retrieval by the
             *        {@link #getCause()} method).  (A null value is permitted,
             *        and indicates that the cause is nonexistent or unknown.)
             */
            CORE_EXPLICIT IOException(const Throwable &cause);

            Object &clone() const override;

        private:
            void raise() && override;
        };

    }
} // core

#endif //CORE23_IOEXCEPTION_H
