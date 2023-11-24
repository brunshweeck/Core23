//
// Created by Brunshweeck on 12/09/2023.
//

#include "StateException.h"
#include <core/private/Unsafe.h>

namespace core {
    using native::Unsafe;

    StateException::StateException(String message) noexcept:
        RuntimeException(Unsafe::moveInstance(message)) {}

    StateException::StateException(String message, const Throwable &cause) noexcept:
    RuntimeException(Unsafe::moveInstance(message), cause) {}

    Object &StateException::clone() const {
        return Unsafe::U.createInstance<StateException>(*this);
    }

    void StateException::raise() &&{
        throw *this;
    }
} // core