//
// Created by Brunshweeck on 12/09/2023.
//

#include "SecurityException.h"

#include <utility>

namespace core {
    SecurityException::SecurityException(String message) CORE_NOTHROW: RuntimeException(std::move(message)) {}

    SecurityException::SecurityException(String message, const Throwable &cause) : RuntimeException(std::move(message)) {}


    void SecurityException::raise() &&{
        throw *this;
    }
} // core