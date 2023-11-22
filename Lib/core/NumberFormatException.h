//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_NUMBERFORMATEXCEPTION_H
#define CORE23_NUMBERFORMATEXCEPTION_H

#include <core/ArgumentException.h>

namespace core {

    /**
     * Thrown to indicate that the application has attempted to convert
     * a string to one of the numeric types, but that the string does not
     * have the appropriate format.
     *
     * @see     Integer.parseInt(String)
     */
    class NumberFormatException: public ArgumentException {
    public:

        /**
         * Constructs a NumberFormatException with no detail message.
         */
        NumberFormatException() CORE_NOTHROW {}

        /**
         * Constructs a NumberFormatException with the specified detail message.
         *
         * @param message
         *          The detail message.
         */
        CORE_EXPLICIT NumberFormatException(String message) CORE_NOTHROW;

        /**
         * Return the sharable copy of this exception
         */
        Object &clone() const override;

        /**
         * Thrown this exception
         */
        void raise() && override;
    };

} // core

#endif //CORE23_NUMBERFORMATEXCEPTION_H
