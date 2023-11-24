//
// Created by Brunshweeck on 12/09/2023.
//

#include "RuntimeException.h"
#include <core/private/Unsafe.h>

namespace core {

    using native::Unsafe;

    RuntimeException::RuntimeException(String message) CORE_NOTHROW
            : Exception(Unsafe::moveInstance(message)) {}

    RuntimeException::RuntimeException(String message, const Throwable &cause) CORE_NOTHROW
            : Exception(Unsafe::moveInstance(message), cause) {}

    Object &RuntimeException::clone() const {
        return Unsafe::U.createInstance<RuntimeException>(*this);
    }

    void RuntimeException::raise() &&{
        throw *this;
    }
} // core