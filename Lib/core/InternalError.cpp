//
// Created by Brunshweeck on 12/09/2023.
//

#include "InternalError.h"

#include <core/private/Unsafe.h>

namespace core {

    using native::Unsafe;

    InternalError::InternalError(String message) CORE_NOTHROW:
            SystemError(Unsafe::moveInstance(message)) {}

    InternalError::InternalError(String message, const Throwable &cause) CORE_NOTHROW:
            SystemError(Unsafe::moveInstance(message), cause) {}

    void InternalError::raise() &&{
        throw *this;
    }

    Object &InternalError::clone() const {
        native::Unsafe::U.createInstance<InternalError>(*this);
    }
} // core