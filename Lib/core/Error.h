//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_ERROR_H
#define CORE23_ERROR_H

#include <core/Throwable.h>

namespace core {

    /**
     * An <b>Error</b> is a subclass of <b>Throwable</b>
     * that indicates serious problems that a reasonable application
     * should not try to catch. Most such errors are abnormal conditions.
     *
     * That is, <b>Error</b> and its subclasses are regarded as unchecked
     * exceptions for the purposes of compile-time checking of exceptions.
     *
     * @author Brunshweeck Tazeussong
     * @see core::Exception
     */
    class Error: public Throwable {
    public:

        /**
         * Constructs a new error with empty String as its detail message.
         * The cause is not initialized.
         */
        Error() CORE_NOTHROW = default;

        /**
         * Constructs a new error with the specified detail message.  The
         * cause is not initialized.
         *
         * @param message
         *            The detail message. The detail message is saved for
         *            later retrieval by the message() method.
         */
        CORE_EXPLICIT Error(String message) CORE_NOTHROW;

        /**
         * Constructs a new error with the specified detail message and
         * cause.
         *
         * @param  message
         *              The detail message (which is saved for later retrieval
         *              by the message() method).
         * @param  cause
         *              The cause (which is saved for later retrieval by the
         *              cause() method)).
         */
        CORE_EXPLICIT Error(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Return sharable copy of this error
         */
        Object &clone() const override;
    private:
        /**
         * Thrown this instance of Error
         */
        void raise() && override;
    };

} // core

#endif //CORE23_ERROR_H
