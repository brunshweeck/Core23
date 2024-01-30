//
// Created by Brunshweeck on 12/09/2023.
//

#include "RuntimeException.h"
#include <core/private/Unsafe.h>

namespace core {

    using namespace native;

    RuntimeException::RuntimeException(String message) CORE_NOTHROW
            : Exception(Unsafe::moveInstance(message)) {}

    RuntimeException::RuntimeException(String message, const Throwable &cause) CORE_NOTHROW
            : Exception(Unsafe::moveInstance(message), cause) {}

    Object &RuntimeException::clone() const {
        return Unsafe::allocateInstance<RuntimeException>(*this);
    }

    void RuntimeException::raise() &&{
        throw RuntimeException(*this);
    }

    RuntimeException::RuntimeException(const Throwable &cause): Exception(cause.toString(), cause) {}

} // core
