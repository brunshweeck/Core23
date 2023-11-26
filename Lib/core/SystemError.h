//
// Created by Brunshweeck on 13/09/2023.
//

#ifndef CORE23_SYSTEMERROR_H
#define CORE23_SYSTEMERROR_H

#include <core/Error.h>

namespace core {

    /**
     * Thrown to indicate that the Process is broken or has
     * run out of resources necessary for it to continue operating.
     *
     *
     * @author  Brunshweeck Tazeussong
     */
    class SystemError: public Error {
    public:
        /**
         * Constructs a <b> SystemError</b> with no detail message.
         */
        SystemError() CORE_NOTHROW = default;

        /**
         * Constructs a <b> SystemError</b> with the specified
         * detail message.
         *
         * @param   message   the detail message.
         */
        CORE_EXPLICIT SystemError(String message) CORE_NOTHROW;

        /**
         * Constructs a <b> SystemError</b> with the specified
         * detail message and cause.  <p>Note that the detail message
         * associated with <b> cause</b> is <i>not</i> automatically
         * incorporated in this error's detail message.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the <b style="color: orange"> message()</b> method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         <b style="color: orange"> cause()</b> method).
         */
        CORE_EXPLICIT SystemError(String message, const Throwable& cause) CORE_NOTHROW;

        Object &clone() const override;

        CORE_NORETURN void raise() && override;
    };

} // core

#endif //CORE23_SYSTEMERROR_H
