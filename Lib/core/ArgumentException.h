//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_ARGUMENTEXCEPTION_H
#define CORE23_ARGUMENTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown to indicate that a method has been passed an illegal or
     * inappropriate argument.
     */
    class ArgumentException : public RuntimeException {
    public:
        /**
         * Constructs an ArgumentException with no detail message.
         */
        ArgumentException() CORE_NOTHROW = default;

        /**
         * Constructs an ArgumentException with the specified detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT ArgumentException(String message) CORE_NOTHROW;

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * @param  message
         *          The detail message (which is saved for later retrieval
         *         by the message() method).
         * @param  cause
         *          The cause (which is saved for later retrieval by the
         *          cause() method).
         */
        CORE_EXPLICIT ArgumentException(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Return sharable copy of this exception
         */
        Object &clone() const override;

        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_ARGUMENTEXCEPTION_H
