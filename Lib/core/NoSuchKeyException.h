//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_NOSUCHKEYEXCEPTION_H
#define CORE23_NOSUCHKEYEXCEPTION_H

#include <core/IllegalArgumentException.h>

namespace core {

    /**
     * Thrown to indicate that map or properties key not found
     */
    class NoSuchKeyException: public IllegalArgumentException {
    public:
        NoSuchKeyException() CORE_NOTHROW = default;

        CORE_EXPLICIT NoSuchKeyException(String message) CORE_NOTHROW;

        CORE_EXPLICIT NoSuchKeyException(const Object &key) CORE_NOTHROW;

        CORE_EXPLICIT NoSuchKeyException(String message, const Throwable &cause) CORE_NOTHROW;

        CORE_EXPLICIT NoSuchKeyException(const Object &key, const Throwable &cause) CORE_NOTHROW;

        Object &clone() const override;

    private:
        CORE_NORETURN void raise() && override;
    };

} // core

#endif //CORE23_NOSUCHKEYEXCEPTION_H
