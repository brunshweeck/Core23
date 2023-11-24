//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_INTERNALERROR_H
#define CORE23_INTERNALERROR_H

#include <core/SystemError.h>

namespace core {

    class InternalError: public SystemError {
    public:

        /**
         * Constructs an InternalError with no detail message.
         */
        InternalError() CORE_NOTHROW = default;

        /**
         * Constructs an InternalError with the specified
         * detail message.
         *
         * @param message
         *            The detail message.
         */
        CORE_EXPLICIT InternalError(String message) CORE_NOTHROW;


        /**
         * Constructs an InternalError with the specified detail
         * message and cause.
         *
         * @param  message
         *              The detail message (which is saved for later retrieval
         *              by the message() method).
         * @param  cause
         *              The cause (which is saved for later retrieval by the
         *              cause() method)).
         */
        CORE_EXPLICIT InternalError(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Return Sharable copy of this error.
         */
        Object &clone() const override;

        /**
         * Thrown this error.
         */
        void raise() && override;
    };

} // core

#endif //CORE23_INTERNALERROR_H
