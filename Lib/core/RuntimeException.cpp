//
// Created by Brunshweeck on 12/09/2023.
//

#include "RuntimeException.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    RuntimeException::RuntimeException(String message) CORE_NOTHROW
            : Exception(U::moveInstance(message)) {}

    RuntimeException::RuntimeException(String message, const Throwable &cause) CORE_NOTHROW
            : Exception(U::moveInstance(message), cause) {}

    Object &RuntimeException::clone() const {
        return U::createInstance<RuntimeException>(*this);
    }

    void RuntimeException::raise() &&{ throw *this; }
} // core