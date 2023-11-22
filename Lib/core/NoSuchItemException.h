//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_NOSUCHITEMEXCEPTION_H
#define CORE23_NOSUCHITEMEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {

    /**
     * Thrown by various accessor methods to indicate that the element being requested
     * does not exist.
     *
     * @see     Iterator.next()
     */
    class NoSuchItemException: public RuntimeException {
    public:
        /**
         * Constructs a <b> NoSuchItemException</b> with empty String
         * as its error message string.
         */
        NoSuchItemException() CORE_NOTHROW {}

        /**
         * Constructs a <b> NoSuchItemException</b> with the specified detail
         * message and cause.
         *
         * @param message     the detail message
         * @param cause the cause (which is saved for later retrieval by the
         *              <b style="color: orange"> cause()</b> method)
         */
        CORE_EXPLICIT NoSuchItemException(String message, const Throwable& cause) CORE_NOTHROW;

        /**
         * Constructs a <b> NoSuchItemException</b>, saving a reference
         * to the error message string <b> s</b> for later retrieval by the
         * <b> getMessage</b> method.
         *
         * @param   message   the detail message.
         */
        CORE_EXPLICIT NoSuchItemException(String message) CORE_NOTHROW;

        Object &clone() const override;

        void raise() && override;
    };

} // core

#endif //CORE23_NOSUCHITEMEXCEPTION_H
