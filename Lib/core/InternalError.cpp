//
// Created by Brunshweeck on 12/09/2023.
//

#include "InternalError.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    InternalError::InternalError(String message) CORE_NOTHROW:
            SystemError(Unsafe::moveInstance(message)) {}

    InternalError::InternalError(String message, const Throwable &cause) CORE_NOTHROW:
            SystemError(Unsafe::moveInstance(message), cause) {}

    void InternalError::raise() &&{
        throw InternalError(*this);
    }

    Object &InternalError::clone() const {
        return Unsafe::allocateInstance<InternalError>(*this);
    }
} // core
