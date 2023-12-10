//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "KeyNotFoundException.h"

namespace core {
    CORE_ALIAS(U, native::Unsafe);

    KeyNotFoundException::KeyNotFoundException(String message) CORE_NOTHROW:
            ArgumentException(U::moveInstance(message)) {}

    KeyNotFoundException::KeyNotFoundException(const Object &key) CORE_NOTHROW:
            ArgumentException(String::valueOf(key)) {}

    KeyNotFoundException::KeyNotFoundException(String message, const Throwable &cause) CORE_NOTHROW:
            ArgumentException(U::moveInstance(message), cause) {}

    KeyNotFoundException::KeyNotFoundException(const Object &key, const Throwable &cause) CORE_NOTHROW
            : ArgumentException(String::valueOf(key), cause) {}

    void KeyNotFoundException::raise() &&{ throw *this; }

    Object &KeyNotFoundException::clone() const {
        return U::createInstance<KeyNotFoundException>(*this);
    }
} // core