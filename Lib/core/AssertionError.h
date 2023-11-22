//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_ASSERTIONERROR_H
#define CORE23_ASSERTIONERROR_H

#include <core/Error.h>

namespace core {

    /**
     * Thrown to indicate that an assertion has failed.
     *
     * <p>
     * The seven one-argument public constructors provided by this
     * class ensure that the assertion error returned by the invocation:
     * <pre>
     *     AssertionError(<i>expression</i>)
     * </pre>
     * has as its detail message the <i>string conversion</i> of
     * <i>expression</i>, regardless of the type of <i>expression</i>.
     */
    class AssertionError : public Error {
    public:

        /**
         * Constructs an AssertionError with no detail message.
         */
        AssertionError() CORE_NOTHROW = default;

        /**
         * Constructs an AssertionError with specified detail message.
         *
         * @param message
         *          The detail message
         */
        CORE_EXPLICIT AssertionError(String message) CORE_NOTHROW;

        template<class Str, Class<gbool>::Iff<Class<Str>::isString()> = true>
        CORE_EXPLICIT AssertionError(Str &&message) CORE_NOTHROW :
                AssertionError(String(message)) {}

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified object, which is converted to a string.
         *<p>
         * If the specified object is an instance of Throwable, it
         * becomes the <i>cause</i> of the newly constructed assertion error.
         *
         * @param expression
         *          Value to be used in constructing detail message.
         */
        CORE_EXPLICIT AssertionError(const Object &expression) CORE_NOTHROW;

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified boolean value, which is converted to a string.
         *
         * @param expression
         *          value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gbool expression) CORE_NOTHROW;

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified char value, which is converted to a string.
         *
         * @param expression
         *          value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gchar expression) CORE_NOTHROW;

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified int value, which is converted to a string.
         *
         * @param expression
         *          value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gint expression) CORE_NOTHROW;

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified long value, which is converted to a string.
         *
         * @param expression
         *          value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(glong expression) CORE_NOTHROW;

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified float value, which is converted to a string.
         *
         * @param expression
         *          value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gfloat expression) CORE_NOTHROW;

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified double value, which is converted to a string.
         *
         * @param expression
         *          value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gdouble expression) CORE_NOTHROW;

        /**
         * Constructs a new AssertionError with the specified detail message and cause.
         *
         * @param  message
         *          The detail message.
         * @param  cause
         *          The cause.
         */
        CORE_EXPLICIT AssertionError(String message, const Throwable &cause) CORE_NOTHROW;

        /**
         * Return sharable copy of error.
         */
        Object &clone() const override;

        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_ASSERTIONERROR_H
