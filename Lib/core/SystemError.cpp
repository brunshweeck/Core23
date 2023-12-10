//
// Created by Brunshweeck on 13/09/2023.
//

#include <core/private/Unsafe.h>
#include "SystemError.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    SystemError::SystemError(String message) CORE_NOTHROW:
            Error(U::moveInstance(message)) {}

    SystemError::SystemError(String message, const Throwable &cause) CORE_NOTHROW:
            Error(U::moveInstance(message), cause) {}

    void SystemError::raise() &&{
        throw *this;
    }

    Object &SystemError::clone() const {
        return U::createInstance<SystemError>(*this);
    }
} // core