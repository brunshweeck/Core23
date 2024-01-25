//
// Created by Brunshweeck on 12/09/2023.
//

#include "IllegalStateException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    IllegalStateException::IllegalStateException(String message) noexcept:
            RuntimeException(Unsafe::moveInstance(message)) {}

    IllegalStateException::IllegalStateException(String message, const Throwable &cause) noexcept:
            RuntimeException(Unsafe::moveInstance(message), cause) {}

    Object &IllegalStateException::clone() const {
        return Unsafe::allocateInstance<IllegalStateException>(*this);
    }

    void IllegalStateException::raise() &&{
        throw IllegalStateException(*this);
    }
} // core
