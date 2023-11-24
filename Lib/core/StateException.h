//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_STATEEXCEPTION_H
#define CORE23_STATEEXCEPTION_H

#include "RuntimeException.h"

namespace core {

    /**
     * Signals that a method has been invoked at an illegal or
     * inappropriate time.  In other words, the environment or
     * application is not in an appropriate state for the requested
     * operation.
     *
     * @author  Brunshweeck Tazeussong
     */
    class StateException: public RuntimeException {
    public:

        /**
         * Constructs an StateException with no detail message.
         * A detail message is a String that describes this particular exception.
         */
        StateException() CORE_NOTHROW = default;

        /**
         * Constructs an StateException with the specified detail
         * message.  A detail message is a String that describes this particular
         * exception.
         *
         * @param message the String that contains a detailed message
         */
        CORE_EXPLICIT StateException(String message) CORE_NOTHROW;

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * <p>Note that the detail message associated with <b>cause</b> is
         * <i>not</i> automatically incorporated in this exception's detail
         * message.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the <b style="color:green;">Throwable.message()</b> method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         <b style="color:green;">Throwable.cause()</b> method).
         */
        CORE_EXPLICIT StateException(String message, const Throwable &cause) CORE_NOTHROW;

        Object &clone() const override;

        void raise() && override;
    };

} // core

#endif //CORE23_STATEEXCEPTION_H
