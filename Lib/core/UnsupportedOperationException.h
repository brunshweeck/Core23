//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_UNSUPPORTEDOPERATIONEXCEPTION_H
#define CORE23_UNSUPPORTEDOPERATIONEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown to indicate that the requested operation is not supported.<p>
     *
     * @author  Brunshweeck Tazeussong
     */
    class UnsupportedOperationException: public RuntimeException {
    public:

        /**
         * Constructs an UnsupportedOperationException with no detail message.
         */
        UnsupportedOperationException() CORE_NOTHROW = default;

        /**
         * Constructs an UnsupportedOperationException with the specified
         * detail message.
         *
         * @param message the detail message
         */
        CORE_EXPLICIT UnsupportedOperationException(String message) CORE_NOTHROW;

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * <p>Note that the detail message associated with <b> cause</b> is
         * <i>not</i> automatically incorporated in this exception's detail
         * message.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the <b style="color: orange"> Throwable.message()</b> method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         <b style="color: orange"> Throwable.cause()</b> method).
         */
        CORE_EXPLICIT UnsupportedOperationException(String message, const Throwable& cause) CORE_NOTHROW;

        Object &clone() const override;

    private:
        void raise() && override;
    };

} // core

#endif //CORE23_UNSUPPORTEDOPERATIONEXCEPTION_H
