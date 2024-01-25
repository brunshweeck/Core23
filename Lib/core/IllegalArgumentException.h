//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_ILLEGALARGUMENTEXCEPTION_H
#define CORE23_ILLEGALARGUMENTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown to indicate that a method has been passed an illegal or
     * inappropriate argument.
     */
    class IllegalArgumentException : public RuntimeException {
    public:
        /**
         * Constructs an IllegalArgumentException with no detail message.
         */
        IllegalArgumentException() CORE_NOTHROW = default;

        /**
         * Constructs an IllegalArgumentException with the specified detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT IllegalArgumentException(String message) CORE_NOTHROW;

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
        CORE_EXPLICIT IllegalArgumentException(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Return sharable copy of this exception
         */
        Object &clone() const override;

    private:
        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_ILLEGALARGUMENTEXCEPTION_H
