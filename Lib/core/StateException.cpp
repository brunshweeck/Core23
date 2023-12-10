//
// Created by Brunshweeck on 12/09/2023.
//

#include "StateException.h"
#include <core/private/Unsafe.h>

namespace core {

    CORE_ALIAS(U, native::Unsafe);

    StateException::StateException(String message) noexcept:
            RuntimeException(U::moveInstance(message)) {}

    StateException::StateException(String message, const Throwable &cause) noexcept:
            RuntimeException(U::moveInstance(message), cause) {}

    Object &StateException::clone() const {
        return U::createInstance<StateException>(*this);
    }

    void StateException::raise() &&{ throw *this; }
} // core