//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_CLONENOTSUPPORTEDEXCEPTION_H
#define CORE23_CLONENOTSUPPORTEDEXCEPTION_H

#include <core/UnsupportedOperationException.h>

namespace core {

    /**
     * Thrown to indicate that the <b> clone</b> method in class
     * <b> Object</b> has been called to clone an object, but that
     * the object's class does not Support the <b> cloning </b>
     * operation.
     * <p>
     * Applications that override the <b> clone</b> method can also
     * throw this exception to indicate that an object could not or
     * should not be cloned.
     *
     * @see     Object.clone()
     */
    class CloneNotSupportedException : public UnsupportedOperationException {

    public:
        /**
         * Constructs a <b> CloneNotSupportedException</b> with no
         * detail message.
         */
        CloneNotSupportedException() CORE_NOTHROW = default;

        /**
         * Constructs a <b> CloneNotSupportedException</b> with the
         * specified detail message.
         *
         * @param  message   the detail message.
         */
        CORE_EXPLICIT CloneNotSupportedException(String message) CORE_NOTHROW;

        Object &clone() const override;

    private:
        CORE_NORETURN void raise() && override;
    };

} // core

#endif //CORE23_CLONENOTSUPPORTEDEXCEPTION_H
