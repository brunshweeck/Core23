//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_EXCEPTION_H
#define CORE23_EXCEPTION_H

#include <core/Throwable.h>

namespace core {

    /**
     * The class <b>Exception</b> and its subclasses are a form of
     * <b>Throwable</b> that indicates conditions that a reasonable
     * application might want to catch.
     *
     * <p>
     * The class <b>Exception</b> and any subclasses that are not also
     * subclasses of <b>RuntimeException</b> are <em>checked exceptions</em>.
     *
     * @author  Brunshweeck Tazeussong
     * @see     core::Error
     */
    class Exception: public Throwable {
    public:

        /**
         * Constructs a new exception with empty String as its detail message.
         * The cause is not initialized.
         */
        Exception() CORE_NOTHROW = default;

        /**
         * Constructs a new exception with the specified detail message.  The
         * cause is not initialized.
         *
         * @param message
         *            The detail message. The detail message is saved for
         *            later retrieval by the message() method.
         */
        CORE_EXPLICIT Exception(String message) CORE_NOTHROW;

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * @param  message
         *              The detail message (which is saved for later retrieval
         *              by the message() method).
         * @param  cause
         *              The cause (which is saved for later retrieval by the
         *              cause() method)).
         */
        CORE_EXPLICIT Exception(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Constructs a new exception with the specified cause and a detail
         * message of <b>cause.toString()</b> (which
         * typically contains the class and detail message of <b>cause</b>).
         * This constructor is useful for exceptions that are little more than
         * wrappers for other throwables.
         *
         * @param  cause the cause (which is saved for later retrieval by the
         *         <b style="color: orange;"> cause()</b> method).
         */
         CORE_EXPLICIT Exception(Throwable const& cause);

        /**
         * Return sharable copy of this exception
         */
        Object &clone() const override;

    private:
        /**
         * Thrown this INSTANCE of Exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_EXCEPTION_H
