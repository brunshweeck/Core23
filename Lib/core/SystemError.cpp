//
// Created by Brunshweeck on 13/09/2023.
//

#include <core/private/Unsafe.h>
#include "SystemError.h"

namespace core {
    using namespace native;

    SystemError::SystemError(String message) CORE_NOTHROW:
            Error(Unsafe::moveInstance(message)) {}

    SystemError::SystemError(String message, const Throwable &cause) CORE_NOTHROW:
            Error(Unsafe::moveInstance(message), cause) {}

    void SystemError::raise() &&{
        throw SystemError(*this);
    }

    Object &SystemError::clone() const {
        return Unsafe::allocateInstance<SystemError>(*this);
    }
} // core
