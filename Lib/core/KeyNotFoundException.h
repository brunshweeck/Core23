//
// Created by Brunshweeck on 12/09/2023.
//

#ifndef CORE23_KEYNOTFOUNDEXCEPTION_H
#define CORE23_KEYNOTFOUNDEXCEPTION_H

#include <core/ArgumentException.h>

namespace core {

    /**
     * Thrown to indicate that map or properties key not found
     */
    class KeyNotFoundException: public ArgumentException {
    public:
        KeyNotFoundException() CORE_NOTHROW = default;

        CORE_EXPLICIT KeyNotFoundException(String message) CORE_NOTHROW;

        CORE_EXPLICIT KeyNotFoundException(const Object &key) CORE_NOTHROW;

        CORE_EXPLICIT KeyNotFoundException(String message, const Throwable &cause) CORE_NOTHROW;

        CORE_EXPLICIT KeyNotFoundException(const Object &key, const Throwable &cause) CORE_NOTHROW;

        Object &clone() const override;

        CORE_NORETURN void raise() && override;
    };

} // core

#endif //CORE23_KEYNOTFOUNDEXCEPTION_H
