//
// Created by Brunshweeck on 12/09/2023.
//

#include <core/private/Unsafe.h>
#include "UnknownError.h"

namespace core {

    using native::Unsafe;

    UnknownError::UnknownError(String message) noexcept:
            Error(Unsafe::moveInstance(message)) {}

    UnknownError::UnknownError(String message, const Throwable &cause) noexcept:
            Error(Unsafe::moveInstance(message), cause) {}

    Object &UnknownError::clone() const {
        return Unsafe::U.createInstance<UnknownError>(*this);
    }

    void UnknownError::raise() &&{
        throw *this;
    }
} // core