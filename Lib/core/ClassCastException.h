//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_CLASSCASTEXCEPTION_H
#define CORE23_CLASSCASTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown to indicate that the code has attempted to cast an object
     * to a subclass of which it is not an INSTANCE.
     */
    class ClassCastException : public RuntimeException {
    public:

        /**
         * Constructs a ClassCastException with no detail message.
         */
        ClassCastException() CORE_NOTHROW = default;

        /**
         * Constructs a ClassCastException with the specified detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT ClassCastException(String message) CORE_NOTHROW;

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

#endif //CORE23_CLASSCASTEXCEPTION_H
