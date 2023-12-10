//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "UnknownError.h"

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    UnknownError::UnknownError(String message) noexcept:
            Error(U::moveInstance(message)) {}

    UnknownError::UnknownError(String message, const Throwable &cause) noexcept:
            Error(U::moveInstance(message), cause) {}

    Object &UnknownError::clone() const {
        return U::createInstance<UnknownError>(*this);
    }

    void UnknownError::raise() &&{ throw *this; }
} // core