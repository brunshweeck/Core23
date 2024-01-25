//
// Created by Brunshweeck on 12/09/2023.
//

#include "IllegalArgumentException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    IllegalArgumentException::IllegalArgumentException(String message) CORE_NOTHROW
            : RuntimeException(Unsafe::moveInstance(message)) {}

    IllegalArgumentException::IllegalArgumentException(String message, const Throwable &cause) CORE_NOTHROW
            : RuntimeException(Unsafe::moveInstance(message), cause) {}

    void IllegalArgumentException::raise() &&{
        throw IllegalArgumentException(*this);
    }

    Object &IllegalArgumentException::clone() const {
        return Unsafe::allocateInstance<IllegalArgumentException>(*this);
    }
} // core
