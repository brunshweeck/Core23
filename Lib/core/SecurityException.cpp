//
// Created by Brunshweeck on 12/09/2023.
//

#include "SecurityException.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    SecurityException::SecurityException(String message) CORE_NOTHROW:
            RuntimeException(U::moveInstance(message)) {}

    SecurityException::SecurityException(String message, const Throwable &cause) :
            RuntimeException(U::moveInstance(message)) {}


    void SecurityException::raise() &&{ throw *this; }

    Object &SecurityException::clone() const {
        return U::createInstance<SecurityException>(*this);
    }
} // core