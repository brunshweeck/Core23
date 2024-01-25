//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_SECURITYEXCEPTION_H
#define CORE23_SECURITYEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown by the security manager to indicate a security violation.
     *
     */
    class SecurityException: public RuntimeException  {
    public:
        /**
         * Constructs a <b> SecurityException</b> with no detail message.
         */
        SecurityException() CORE_NOTHROW = default;

        /**
         * Constructs a <b> SecurityException</b> with the specified
         * detail message.
         *
         * @param   message   the detail message.
         */
        CORE_EXPLICIT SecurityException(String message) CORE_NOTHROW;

        /**
         * Creates a <b> SecurityException</b> with the specified
         * detail message and cause.
         *
         * @param message the detail message (which is saved for later retrieval
         *        by the <b style="color: orange"> message()</b> method).
         * @param cause the cause (which is saved for later retrieval by the
         *        <b style="color: orange"> cause()</b> method).
         */
        CORE_EXPLICIT SecurityException(String message, const Throwable& cause);

        Object &clone() const override;

    private:
        CORE_NORETURN void raise() && override;
    };

} // core

#endif //CORE23_SECURITYEXCEPTION_H
