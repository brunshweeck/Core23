//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_CASTEXCEPTION_H
#define CORE23_CASTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown to indicate that the code has attempted to cast an object
     * to a subclass of which it is not an instance.
     */
    class CastException : public RuntimeException {
    public:

        /**
         * Constructs a CastException with no detail message.
         */
        CastException() CORE_NOTHROW = default;

        /**
         * Constructs a CastException with the specified detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT CastException(String message) CORE_NOTHROW;

        /**
         * Return sharable copy of this exception
         */
        Object &clone() const override;

        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_CASTEXCEPTION_H
