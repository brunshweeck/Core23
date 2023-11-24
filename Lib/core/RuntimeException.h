//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_RUNTIMEEXCEPTION_H
#define CORE23_RUNTIMEEXCEPTION_H

#include <core/Exception.h>

namespace core {

    /**
     * <b>RuntimeException</b> is the superclass of those
     * exceptions that can be thrown during the normal operation.
     *
     * <p>
     * <b>RuntimeException</b> and its subclasses are <em>unchecked
     * exceptions</em>.
     *
     * @author  Brunshweeck Tazeussong
     */
    class RuntimeException : public Exception {
    public:

        /**
         * Constructs a new runtime exception with empty String as its
         * detail message.  The cause is not initialized.
         */
        RuntimeException() CORE_NOTHROW = default;

        /**
         * Constructs a new runtime exception with the specified detail message.
         * The cause is not initialized.
         *
         * @param message
         *           The detail message. The detail message is saved for
         *           later retrieval by the message() method.
         */
        CORE_EXPLICIT RuntimeException(String message) CORE_NOTHROW;

        /**
         * Constructs a new runtime exception with the specified detail message and
         * cause.
         *
         * @param message
         *           The detail message. The detail message is saved for
         *           later retrieval by the message() method.
         * @param  cause
         *          The cause (which is saved for later retrieval by the cause() method.
         */
        CORE_EXPLICIT RuntimeException(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Return sharable copy of this exception.
         */
        Object &clone() const override;

        /**
         * Thrown this exception.
         */
        CORE_NORETURN void raise() && override;
    };

} // core

#endif //CORE23_RUNTIMEEXCEPTION_H
