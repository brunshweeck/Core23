//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_ARITHMETICEXCEPTION_H
#define CORE23_ARITHMETICEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown when an exceptional arithmetic createCondition has occurred.
     */
    class ArithmeticException : public RuntimeException {
    public:

        /**
         * Constructs an ArithmeticException with no detail
         * message.
         */
        ArithmeticException() CORE_NOTHROW = default;

        /**
         * Constructs an ArithmeticException with the specified
         * detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT ArithmeticException(String message) CORE_NOTHROW;

        /**
         * Return sharable copy of this exception.
         */
        Object &clone() const override;

        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_ARITHMETICEXCEPTION_H
