//
// Created by Brunshweeck on 12/09/2023.
//

#include "KeyNotFoundException.h"

namespace core {
    KeyNotFoundException::KeyNotFoundException(String message) CORE_NOTHROW: ArgumentException(message) {}

    KeyNotFoundException::KeyNotFoundException(const Object &key) CORE_NOTHROW: ArgumentException(String::valueOf(key)) {}

    KeyNotFoundException::KeyNotFoundException(String message, const Throwable &cause) CORE_NOTHROW: ArgumentException(message, cause) {}

    KeyNotFoundException::KeyNotFoundException(const Object &key, const Throwable &cause) CORE_NOTHROW: ArgumentException(String::valueOf(key), cause) {}

    void KeyNotFoundException::raise() &&{
        throw *this;
    }
} // core